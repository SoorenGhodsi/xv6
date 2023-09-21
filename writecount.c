#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
    // Get the current write count
    int count = writecount();
    printf(1, "Initial write count: %d\n", count);

    // If an argument is provided, use it to set the write counter
    if(argc == 2){
        int new_count = atoi(argv[1]);
        printf(1, "Setting write count to %d...\n", new_count);
        setwritecount(new_count);

        count = writecount();
        printf(1, "Write count after setting: %d\n", count);
    } else { // Invalid number of arguments
        printf(2, "usage: writecount [new_count]\n");
        exit();
    }

    // Perform a test write and get the write count
    write(1, "This is a test write\n", 22);
    count = writecount();

    // Display final write count and exit
    printf(1, "Write count after write: %d\n", count);
    exit();
}
