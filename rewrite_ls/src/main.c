#include <dirent.h>
#include <grp.h>
#include <inttypes.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

// for the arguments we define flags:
#define ALL (1 << 0)
#define LONG_LISTING (1 << 1)

//  helper function: input: mode, outputs the mode in string
static void mode_str(mode_t m, char* out)
{
    out[0] = S_ISDIR(m) ? 'd' : S_ISLNK(m) ? 'l' : '-';
    out[1] = (m & S_IRUSR) ? 'r' : '-';
    out[2] = (m & S_IWUSR) ? 'w' : '-';
    out[3] = (m & S_IXUSR) ? 'x' : '-';
    out[4] = (m & S_IRGRP) ? 'r' : '-';
    out[5] = (m & S_IWGRP) ? 'w' : '-';
    out[6] = (m & S_IXGRP) ? 'x' : '-';
    out[7] = (m & S_IROTH) ? 'r' : '-';
    out[8] = (m & S_IWOTH) ? 'w' : '-';
    out[9] = (m & S_IXOTH) ? 'x' : '-';
    out[10] = '\0';
}
// TODO: maybe put here the next helper function for how many bytes the file is?

//  main funtion :)
int main(int argc, char** argv)
{
    uint8_t what_to_do = 0;

    // Firstly we do the parsing:
    if (argc >= 2) {
        // now we want to parse it
        char* arg = argv[1];
        bool scanning = 0;
        for (size_t size = 0; arg[size] != '\0'; size++) {
            char current = arg[size];
            if (current == '-') {
                // start scanning for chars
                scanning = true;
                continue;
            }
            if (scanning) {
                if (current == 'l') {
                    // turn on flag LONG_LISTING
                    what_to_do = LONG_LISTING | what_to_do;
                } else if (current == 'a') {
                    // Turn on flag ALL
                    what_to_do = ALL | what_to_do;
                }
            }
        }
    }
    DIR* dir = opendir(".");
    if (!dir) {
        perror("opendir");
        return 1;
    }
    struct dirent* entry;
    // filter:
    while ((entry = readdir(dir)) != NULL) {
        if (what_to_do == 0 && (entry->d_name[0] != '.')) {
            printf("%s\n", entry->d_name);
        }
        // if flage -a is set just do nothin
        if ((what_to_do & ALL) != 0) {
            printf("%s\n", entry->d_name);
        }
        // if flag -l is set:
        if ((what_to_do & LONG_LISTING) != 0) {
            struct stat st;
            char path[PATH_MAX];
            snprintf(path, sizeof path, "./%s", entry->d_name);
            if (lstat(path, &st) == -1) {
                perror("lstat");
                continue;
            }
            char out[11];
            mode_str(st.st_mode, out);
            struct passwd* pw = getpwuid(st.st_uid);
            struct group* gr = getgrgid(st.st_gid);
            const char* owner = pw ? pw->pw_name : "?";
            const char* group = gr ? gr->gr_name : "?";
            char timestr[64];
            struct tm* tm = localtime(&st.st_mtime);
            strftime(timestr, sizeof timestr, "%b %e %H:%M", tm);
            printf("%s %s %s %s %s\n", out, owner, group, timestr,
                   entry->d_name);
        }
    }
    closedir(dir);
    return 1;
}
