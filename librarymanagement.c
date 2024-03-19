#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct books
{
    int id;
    char bookname[50];
    char authorname[50];
    char date[12];
    int isIssued; // Add this line to track book issuance status
} b;

struct student
{
    int id;
    char sname[50];
    char sclass[50];
    int sRoll;
    char bookname[50];
    char date[12];
};

FILE *fp;

void addBook()
{
    struct books b;
    FILE *fp; // Declare file pointer locally

    printf("Enter Book ID: ");
    scanf("%d", &b.id);
    printf("Enter Book Name: ");
    scanf(" %[^\n]", b.bookname); // Read book name with spaces
    printf("Enter Author Name: ");
    scanf(" %[^\n]", b.authorname); // Read author name with spaces

    // Generating current date
    time_t t;
    struct tm *tm_info;
    time(&t);
    tm_info = localtime(&t);
    strftime(b.date, sizeof(b.date), "%Y-%m-%d", tm_info);

    b.isIssued = 0; // Initialize issuance status to 0

    // Open file in append mode
    fp = fopen("books.dat", "ab");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    // Write book information to file
    fwrite(&b, sizeof(struct books), 1, fp);

    printf("Book added successfully!\n");
    fclose(fp);
}


void Booklist()
{
    struct books b;
    fp = fopen("books.dat", "rb");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    printf("Book List:\n");
    while (fread(&b, sizeof(struct books), 1, fp) == 1)
    {
        printf("Book ID: %d\n", b.id);
        printf("Book Name: %s\n", b.bookname);
        printf("Author Name: %s\n", b.authorname);
        printf("Date Added: %s\n", b.date);
        printf("Issued: %s\n", b.isIssued ? "Yes" : "No");
        printf("---------------------------\n");
    }

    fclose(fp);
}

void del()
{
    int bookID;
    printf("Enter Book ID to remove: ");
    scanf("%d", &bookID);

    FILE *tempFp;
    tempFp = fopen("temp.dat", "wb");

    fp = fopen("books.dat", "rb");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    struct books tempBook;
    while (fread(&tempBook, sizeof(struct books), 1, fp) == 1)
    {
        if (tempBook.id != bookID)
        {
            fwrite(&tempBook, sizeof(struct books), 1, tempFp);
        }
    }

    fclose(fp);
    fclose(tempFp);

    remove("books.dat");
    rename("temp.dat", "books.dat");

    printf("Book removed successfully!\n");
}

void issuebook()
{
    int bookID, studentID;
    printf("Enter Book ID to issue: ");
    scanf("%d", &bookID);

    fp = fopen("books.dat", "rb+");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    struct books tempBook;
    while (fread(&tempBook, sizeof(struct books), 1, fp) == 1)
    {
        if (tempBook.id == bookID && tempBook.isIssued == 0) // Check if the book is not already issued
        {
            tempBook.isIssued = 1; // Mark the book as issued

            // Collect student details
            struct student s;
            printf("Enter Student ID: ");
            scanf("%d", &s.id);
            printf("Enter Student Name: ");
            scanf("%s", s.sname);
            printf("Enter Student Class: ");
            scanf("%s", s.sclass);
            printf("Enter Student Roll Number: ");
            scanf("%d", &s.sRoll);

            // Update book details with student information
            strcpy(tempBook.bookname, s.sname);
            strcpy(tempBook.date, tempBook.date);

            // Save changes to file
            fseek(fp, -sizeof(struct books), SEEK_CUR);
            fwrite(&tempBook, sizeof(struct books), 1, fp);
            fclose(fp);

            printf("Book issued successfully to %s!\n", s.sname);
            return;
        }
    }

    fclose(fp);
    printf("Book with ID %d not found or already issued.\n", bookID);
}


void issuelist()
{
    struct books b;
    fp = fopen("books.dat", "rb");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    printf("Issued Book List:\n");
    while (fread(&b, sizeof(struct books), 1, fp) == 1)
    {
        if (b.isIssued)
        {
            printf("Book ID: %d\n", b.id);
            printf("Book Name: %s\n", b.bookname);
            printf("Author Name: %s\n", b.authorname);
            printf("Date Issued: %s\n", b.date);
            printf("---------------------------\n");
        }
    }

    fclose(fp);
}

void editBook()
{
    int bookID;
    printf("Enter Book ID to edit: ");
    scanf("%d", &bookID);

    fp = fopen("books.dat", "rb+");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    struct books tempBook;
    while (fread(&tempBook, sizeof(struct books), 1, fp) == 1)
    {
        if (tempBook.id == bookID)
        {
            printf("Enter new Book Name: ");
            scanf("%s", tempBook.bookname);
            printf("Enter new Author Name: ");
            scanf("%s", tempBook.authorname);

            fseek(fp, -sizeof(struct books), SEEK_CUR);
            fwrite(&tempBook, sizeof(struct books), 1, fp);
            fclose(fp);

            printf("Book details updated successfully!\n");
            return;
        }
    }

    fclose(fp);
    printf("Book with ID %d not found.\n", bookID);
}
int main()
{
    int ch;
    while (1)
    {
        system("cls");
        printf("Library Management System\n");
        printf("1. Add Book\n");
        printf("2. Book List\n");
        printf("3. Remove Book\n");
        printf("4. Issue Book\n");
        printf("5. Issued Book List\n");
        printf("6. Edit Book Details\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);

        switch (ch)
        {
        case 0:
            exit(0);
            break;
        case 1:
            addBook();
            break;
        case 2:
            Booklist();
            break;
        case 3:
            del();
            break;
        case 4:
            issuebook();
            break;
        case 5:
            issuelist();
            break;
        case 6:
            editBook();
            break;
        default:
            printf("Invalid choice. Please choose from the menu only.\n");
            break;
        }

        printf("Press any key to continue...");
        getchar(); // consume newline character
        getchar();
    }

    return 0;
}
