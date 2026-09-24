#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#define TARGET_FILE "/tmp/73_target.txt"
#define HARD_LINK_FILE "/tmp/73_hardlink.txt"
#define SYMLINK_FILE "/tmp/73_symlink.txt"

/*****************************************************************************
 * Name: printStatInfo
 *
 * Description:
 *         Prints a path's inode number and hard-link count via stat().
 *
 * Inputs:
 *         label : descriptive label to print alongside the info.
 *         path  : filesystem path to stat().
 *
 * Returns:
 *         None.
 *****************************************************************************/
void printStatInfo(const char *label, const char *path)
{
    struct stat statBuf;

    if (stat(path, &statBuf) == 0)
    {
        printf("%s: inode = %lu, link count = %lu\n", label,
               (unsigned long)statBuf.st_ino, (unsigned long)statBuf.st_nlink);
    }
    else
    {
        perror(label);
    }
}

/*****************************************************************************
 * Name: main
 *
 * Description:
 *         Creates a file, a hard link, and a symbolic link to it, then
 *         inspects inode numbers and link counts via stat()/lstat() to
 *         show that a hard link shares the same inode (and bumps the link
 *         count) while a symlink is a distinct inode holding a path string.
 *
 * Returns:
 *         0 on success.
 *****************************************************************************/
int main()
{
    FILE *file;
    struct stat symlinkStat;

    file = fopen(TARGET_FILE, "w");
    if (file == NULL)
    {
        perror("fopen");
        return 1;
    }
    fprintf(file, "demo content\n");
    fclose(file);

    unlink(HARD_LINK_FILE);
    unlink(SYMLINK_FILE);
    link(TARGET_FILE, HARD_LINK_FILE);
    symlink(TARGET_FILE, SYMLINK_FILE);

    printStatInfo("target (before hard link)", TARGET_FILE);
    printStatInfo("hard link (same inode, link count 2)", HARD_LINK_FILE);

    // lstat(), unlike stat(), does not follow a symlink - it reports on the
    // symlink object itself, which has its own distinct inode.
    if (lstat(SYMLINK_FILE, &symlinkStat) == 0)
    {
        printf("symlink itself: inode = %lu (distinct from target's inode)\n",
               (unsigned long)symlinkStat.st_ino);
    }
    printStatInfo("symlink, followed (same inode as target)", SYMLINK_FILE);

    unlink(TARGET_FILE);
    unlink(HARD_LINK_FILE);
    unlink(SYMLINK_FILE);

    return 0;
}
