#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Single-exit "goto cleanup" idiom: acquire resources in order, release them
// in reverse via fall-through labels. Each failure jumps to the label that
// frees only what was already acquired. Used throughout the Linux kernel.
static int copy_first_line(const char *path, char **out)
{
    int rc = -1;
    FILE *f = NULL;
    char *buf = NULL;

    f = fopen(path, "r");
    if (!f)
    {
        rc = -errno;
        goto out;
    }

    buf = malloc(256);
    if (!buf)
    {
        rc = -ENOMEM;
        goto out_close;
    }

    if (!fgets(buf, 256, f))
    {
        rc = -EIO;
        goto out_free;
    }
    buf[strcspn(buf, "\n")] = '\0';

    *out = buf;     // ownership transferred to the caller
    buf = NULL;     // so the cleanup below must not free it
    rc = 0;

out_free:
    free(buf);      // free(NULL) is a no-op
out_close:
    fclose(f);
out:
    return rc;
}

int main(void)
{
    char *line = NULL;
    int rc = copy_first_line("/etc/hostname", &line);
    if (rc == 0)
    {
        printf("first line: %s\n", line);
        free(line);
    }
    else
        printf("failed: %s\n", strerror(-rc));

    rc = copy_first_line("/no/such/file", &line);
    printf("missing file -> rc=%d (%s)\n", rc, strerror(-rc));
    return 0;
}
