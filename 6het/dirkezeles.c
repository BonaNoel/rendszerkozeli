#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

// 1 függvény hogy a alapértelmezett könyvtár kilistázzaé s beolvassa szting
// ha sztring alkönyvtár név akkor azt is listázza ki
// ha file akkor nyissa meg fopen olvasásba és a fáljleiórájt adja vissza

FILE *browseForOpen()
{
    DIR *d;
    FILE *f;
    char name[256];
    struct dirent *entry;
    chdir(getenv("HOME"));

    do
    {
        d = opendir(".");
        while ((entry = readdir(d)) != NULL)
        {
            printf("%s\n", (*entry).d_name);
        }
        closedir(d);
        printf("\n\b Choose one of them: ");
        scanf("%s", name);
        if (stat(name, &inode) < 0)
        {
            continue;
        }
        if (inode.st_mode & S_IFREG)
        {
            break;
        }
        chdir(name);

    } while (1);

    return (fopen(name, "r"));
}

int main()
{
    FILE *f;
    char c;
    f = browseForOpen();
    while (!feof(f))
    {
        fscanf(f, "%c", &c);
        printf("%c", c);
    }
    fclose(f);

    return 0;
}
