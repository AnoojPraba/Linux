# Computer Boot Process

Notes-only: this is a hardware/firmware sequencing story, not something a
userspace C program on an already-booted machine can demonstrate - a
runnable demo would either be trivial or need to run before an OS exists at
all. Sits right before `02_Processes` because "process" as a concept only
exists once the kernel has booted and started PID 1 - this folder covers
everything that happens before that.

## The stages, in order

1. **Power supply initialization** - the PSU brings the motherboard, CPU,
   RAM, storage, and cooling up to stable voltages before anything else can
   run. If this fails or is unstable, nothing downstream even gets a chance
   to run.
2. **BIOS/UEFI startup and POST** - the firmware chip (soldered to the
   motherboard, independent of any disk) runs first. It performs the
   Power-On Self-Test (POST): checks RAM, CPU, video adapter, storage
   controllers are present and sane. POST beep codes exist because at this
   point there may be no working video output yet - a bad RAM stick or dead
   GPU has to be signaled some other way. After POST passes, firmware
   enumerates and configures the connected devices (assigns resources,
   builds device tables) before handing off further.
3. **Loading the boot loader** - firmware walks the configured boot order
   looking for a bootable device.
   - Legacy BIOS reads the **MBR** (Master Boot Record), the first 512
     bytes of the boot disk, which contains a tiny bit of boot code plus a
     partition table.
   - Modern systems use **UEFI** with **GPT** (GUID Partition Table) and a
     dedicated **EFI System Partition (ESP)** holding the actual bootloader
     binaries.
   - Either way, the boot loader itself (GRUB/LILO on Linux, Windows Boot
     Manager on Windows) is a small program with one job: locate the OS
     kernel image on disk and load it into RAM.
4. **Kernel and init process** - the kernel is loaded into memory and takes
   over: sets up memory management, initializes CPU scheduling structures,
   loads device drivers, mounts the root filesystem, then starts **PID 1**
   (init/systemd). Init decides the target run level/target - legacy SysV
   used numbered runlevels (3 = multi-user text mode, 5 = graphical),
   systemd replaces these with named targets (`multi-user.target`,
   `graphical.target`) that are really just dependency-ordered sets of
   units.
5. **System services and daemons** - init/systemd brings up background
   services in dependency order: networking, logging, cron, display
   manager, etc. "Dependency order" here is the same idea as topological
   sort over a DAG of unit dependencies.
6. **User login and shell/desktop** - after authentication, either a shell
   (text login) or a full desktop environment (graphical login/display
   manager) is started as the user's entry point.

## BIOS vs UEFI

| | BIOS (legacy) | UEFI |
|---|---|---|
| Partition table | MBR | GPT |
| Max primary partitions | 4 | effectively unlimited (128 typical) |
| Max disk size | 2 TB | far beyond 2 TB (64-bit LBA addressing) |
| CPU mode at boot | 16-bit real mode | can run in 32/64-bit mode directly |
| Pre-OS environment | minimal, boot-code-in-512-bytes only | richer - has its own drivers, shell, can run `.efi` apps |
| Security | none built in | **Secure Boot** - firmware verifies bootloader/kernel signatures before running them, to block unsigned/tampered boot code |

- Secure Boot is a chain-of-trust concept: firmware trusts a set of keys,
  and only runs bootloader images signed by a trusted key - the same
  general idea as any code-signing/verification chain, just applied at the
  earliest possible point.
- UEFI's richer pre-OS environment (its own drivers, shell, ability to run
  `.efi` binaries) is why UEFI setup screens can do things BIOS setup
  screens couldn't - it's effectively a tiny pre-OS operating environment,
  not just a fixed self-test-and-jump routine.

## Cross-references to other folders

- **`02_Processes`/`04_ProcessLifecycle`**: the concept of a "process" does
  not exist until step 4 creates PID 1. Everything in this folder happens
  before there is a process table at all.
- **`06_SignalHandling`**: init/PID 1 has a special role in signal handling
  - it's the ancestor every orphaned process gets reparented to, and it's
  responsible for reaping zombies system-wide, not just for its own
  children.
- **`59_LinkerAndLoaderMechanics`**: a boot loader is conceptually a much
  more primitive version of a program loader - both read code from
  storage, place it in memory, and transfer control to it. The boot loader
  just does this with no OS underneath to help (no `exec()`, no dynamic
  linker, no relocation) - it hand-loads a flat kernel image and jumps to
  its entry point directly.
- **`55_FilesystemInternals`**: "mounting the root filesystem" during
  kernel init means the kernel is now walking inode structures to resolve
  `/` and everything under it - the mount step is where the filesystem
  layer this folder describes actually becomes available for use.
- **`32_VirtualMemoryDeepDive`**: "kernel initializes memory management"
  in step 4 is where page tables, the virtual address space layout, and
  paging structures described there first get set up - before this point,
  addressing is still firmware-managed, not virtual-memory-managed.

## Interview framing

"What happens when you turn on a computer / press the power button on your
laptop?" is a classic open-ended systems question. A strong answer is not
just a list of stage names - it explains *why* each stage has to exist:

- Power has to stabilize before logic runs at all (hardware constraint).
- POST exists because you can't trust software correctness checks on
  hardware that hasn't been verified to work yet.
- The bootloader exists because the kernel is too large/complex to be
  loaded directly by firmware - firmware only needs to know how to load a
  small, simple program, and that program's whole job is finding and
  loading the real kernel.
- The kernel initializes core subsystems (memory, scheduling, drivers, FS)
  before starting init, because init and every process after it depends on
  those subsystems already working.
- init/systemd exists to bring up everything else in the right dependency
  order rather than all at once.

Being able to narrate the *reasons*, not just recite the stage names, is
what separates a strong answer from a memorized list.
