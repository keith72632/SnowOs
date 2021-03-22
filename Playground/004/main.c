#include <stdio.h>

char array[256];

int string_length(char s[])
{
    int i;
    for(i = 0; s[i] != '\0'; i++) return i;
}

void append(char s[], char n, short *index)
{
    s[*index] = n;
    s[*index + 1] = '\0';
    *index+=1;
}

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

void clear_array(char str[], short *ctr)
{
    for(int i = 0; i < *ctr; i++) str[i] == '\0';
}

int main()
{
    /*
    char str[] = "EXIT";
    int i = compare_string(str, "EXIT");
    printf("%d\n", i);
    if(i == 0){
        execute_command(str);
    }
    */
    short ctr = 0;
    printf("Array before append:\n");
    printf("%s\n", array);
    append(array, 'h', &ctr);
    printf("Array after append:\n");
    printf("%s\n", array);
    append(array, 'i', &ctr);
    printf("Array after append:\n");
    printf("%s\n", array);
    append(array, 'i', &ctr);
    printf("Array after append:\n");
    printf("%s\n", array);
    clear_array(array, &ctr);
    printf("Array after cleared:\n");
    printf("%s\n", array);

    return 0;
}