#include <stdio.h>

int compare_string(char s1[], char s2[]) {
    int i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}

void execute_command(char *input)
{
    if(compare_string(input, "EXIT") == 0){
        printf("Stopping the CPU. Bye\n");
        printf("this should be haulting\n");
    }
    printf("Unkown Command");
    printf("%s\n", input);
    printf("\n>");
}

int main()
{
    char str[] = "EXIT";
    int i = compare_string(str, "EXIT");
    printf("%d\n", i);
    if(i == 0){
        execute_command(str);
    }
}