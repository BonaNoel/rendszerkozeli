// bináris fáljkezelés

#include <unistd.h>   // opne()
#include <fcntl.h>    // file controll
#include <sys/stat.h> // jogosultság

int main()
{
    int adat;
    int f;
    f = open("abc.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (f < 0)
    {
        write(2, "error", 5);
        return 1;
    }

    lseek(f, 4, SEEK_SET);

    int n = read(0, &adat, sizeof(int));
    // if(n==0) eléreted a fálj végét
    close(f);

    return 0;
}
// write()
// 0 stdin
// 1 stdout
// 2 stderr

// opne()
// O_RDONLY  read
// O_WRONLY write
// O_RDWR read+write

// O_CREAT create file ha nem létezik
// O_TRUNC töröld a tartalmát ha létezett
// O_APPEND append

// S_IRUSR usr:r
// S_IWUSR usr:w
// S_IXUSR usr:x
// S_IRGRP group:r
// S_IWGRP group:w
// S_IXGRP group:x
// S_IROTH other:r
// S_IWOTH other:w
// S_IXOTH other:x