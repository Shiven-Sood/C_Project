#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000
#define PASSWORD "1234"  
#define SHIFT 3           


void caesarCipher(char *text, int shift) 
{
    for (int i = 0; text[i] != '\0'; i++) 
{
        char c = text[i];
        if (c >= 'a' && c <= 'z') {
            c = ((c - 'a' + shift + 26) % 26) + 'a';
        } else if (c >= 'A' && c <= 'Z') {
            c = ((c - 'A' + shift + 26) % 26) + 'A';
        }
        text[i] = c;
    }
}

void addEntry() 
{
    char date[20], entry[MAX];
    FILE *fp;

    printf("Enter date (dd-mm-yyyy): ");
    scanf("%19s", date);
    getchar();  

    printf("Write your diary entry:\n");
    fgets(entry, MAX, stdin);
    entry[strcspn(entry, "\n")] = '\0';

    caesarCipher(entry, SHIFT); 

    fp = fopen("diary.txt", "a");
    if (fp == NULL) 
    {
        printf("Error opening file!\n");
        return;
    }
    fprintf(fp, "%s:%s\n", date, entry);
    fclose(fp);

    printf("Entry saved successfully!\n");
}


void viewEditEntry() 
{
    char date[20], line[MAX], newEntry[MAX];
    FILE *fp, *temp;
    int found = 0;
    printf("Enter date to view/edit (dd-mm-yyyy): ");
    scanf("%19s", date);
    getchar();
    fp = fopen("diary.txt", "r");
    if (fp == NULL) 
    {
        printf("No diary file found. Please add an entry first.\n");
        return;
    }
    temp = fopen("temp.txt", "w");
    if (temp == NULL) 
    {
        printf("Error creating temp file!\n");
        fclose(fp);
        return;
    }
    while (fgets(line, MAX, fp)) 
    {
        char fileDate[20], entry[MAX];
        if (sscanf(line, "%19[^:]:%[^\n]", fileDate, entry) == 2) 
        {
            if (strcmp(fileDate, date) == 0) 
            {
                found = 1;
                caesarCipher(entry, -SHIFT); 
                printf("Diary entry on %s:\n%s\n", date, entry);

                printf("Do you want to edit this entry? (y/n): ");
                char choice;
                scanf(" %c", &choice);
                getchar();
                if (choice == 'y' || choice == 'Y') 
                {
                    printf("Enter new diary entry:\n");
                    fgets(newEntry, MAX, stdin);
                    newEntry[strcspn(newEntry, "\n")] = '\0';
                    caesarCipher(newEntry, SHIFT);
                    fprintf(temp, "%s:%s\n", date, newEntry);
                    continue;
                }
            }
        }
        fprintf(temp, "%s", line);
    }

    fclose(fp);
    fclose(temp);

    if (found) 
    {
        remove("diary.txt");
        rename("temp.txt", "diary.txt");
        printf("Changes saved successfully!\n");
    } 
    else
     {
        remove("temp.txt");
        printf("No entry found for this date.\n");
    }
}


int main() 
{
    char inputPass[20];
    int choice;
    printf("Enter password: ");
    scanf("%19s", inputPass);
    if (strcmp(inputPass, PASSWORD) != 0) 
    {
        printf("Incorrect password! Exiting...\n");
        return 0;
    }
    do 
    {
        printf("\nPersonal Diary Menu:\n");
        printf("1. Add Entry\n");
        printf("2. View/Edit Entry by Date\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) 
        {
            case 1:
                addEntry();
                break;
            case 2:
                viewEditEntry();
                break;
            case 3:
                printf("Exiting diary. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
     while (choice != 3);

    return 0;
}
