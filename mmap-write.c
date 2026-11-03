// Write a Random Number to a Memory-Mapped File
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define FILE_LENGTH 0x100 // 0x คือเลลขฐาน 16 (256)

/* Return a uniformly random number in the range [low,high]. */
int random_range(unsigned const low, unsigned const high)
{
    unsigned const range = high - low + 1; // หาขนาดของช่วง high - low + 1
    return low + (int)(((double)range) * rand() / (RAND_MAX + 1.0)); //  คูณด้วยอัตราส่วน(ที่สุ่มมา)
}
int main(int argc, char *const argv[])
{
    int fd;
    void *file_memory;
    /* Seed the random number generator. */
    srand(time(NULL));

    /* Prepare a file large enough to hold an unsigned integer. */
    fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR); // create file 1 file (can Read and Write) ขนาดไฟล์ 0 ไบต์ (Low Level)
    // ถ้าใช้  fopen --> High Levvel
    // 2 บรรทัดด้านล่างเซตขนาดของไฟล์ ขนาด 257 ไบต์ (this is Low Level) แต่อันนี้ต้องใช้ low เพราะ open is low level
    lseek(fd, FILE_LENGTH + 1, SEEK_SET); // ไปที่ตำแหน่ง file_lenght (256 + 1 = 257) +1 คือเอาไว้เผื่อสำหรับ null character ไปที่ต้นไฟล์ (SEEK_SET)
    write(fd, "", 1); //
    lseek(fd, 0, SEEK_SET); // กลับมาที่ต้นไฟล์ เพื่อเริ่มเขียนที่ต้นไฟล์ ไม่งั้นมันจะอยู่ท้ายไฟล์อยู่
    /* Create the memory mapping. */
    file_memory = mmap(0, FILE_LENGTH, PROT_WRITE, MAP_SHARED, fd, 0); //  Map file ที่พึ่งสร้างเสร็จ
    close(fd);
    /* Write a random integer to memory-mapped area. */
    sprintf((char *)file_memory, "% d\n", random_range(-100, 100)); // เขียนค่า random เขียนที่ตำแหน่งแรกใช่ memory
    /* Release the memory (unnecessary because the program exits). */
    munmap(file_memory, FILE_LENGTH); // map แล้วต้อง unmap ด้วย
    return 0;
}