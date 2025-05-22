#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BOOKS 100

typedef struct {
    int id;
    char title[100];
    char author[100];
    char category[50];
    int isBorrowed;
    int borrowDay, borrowMonth, borrowYear;
    char filepath[100]; // File to open
} Book;

Book library[MAX_BOOKS];
int count = 0;

int daysInMonth(int month, int year) {
    if (month == 2)
        return (year % 4 == 0) ? 29 : 28;
    if (month == 4 || month == 6 || month == 9 || month == 11)
        return 30;
    return 31;
}

int dateDifference(int d1, int m1, int y1, int d2, int m2, int y2) {
    if (y1 == y2 && m1 == m2)
        return d2 - d1;
    return (d2 + daysInMonth(m1, y1) - d1);
}

void preloadBooks() {
    Book preset[] = {
        {1, "To Kill a Mockingbird", "Harper Lee", "Fiction", 0, 0, 0, 0, "books/tokillamockingbird.txt"},
        {2, "1984", "George Orwell", "Fiction", 0, 0, 0, 0, "books/1984.txt"},
        {3, "The Great Gatsby", "F. Scott Fitzgerald", "Fiction", 0, 0, 0, 0, "books/gatsby.txt"},
        {4, "Becoming", "Michelle Obama", "Autobiography", 0, 0, 0, 0, "books/becoming.txt"},
        {5, "Long Walk to Freedom", "Nelson Mandela", "Autobiography", 0, 0, 0, 0, "books/mandela.txt"},
        {6, "Playing It My Way", "Sachin Tendulkar", "Sports", 0, 0, 0, 0, "books/playingitmyway.txt"},
        {7, "Open", "Andre Agassi", "Sports", 0, 0, 0, 0, "books/open.txt"}
    };
    count = sizeof(preset) / sizeof(preset[0]);
    for (int i = 0; i < count; i++) {
        library[i] = preset[i];
    }
}

void viewByCategory(const char *category) {
    printf("\nBooks in category: %s\n", category);
    for (int i = 0; i < count; i++) {
        if (strcmp(library[i].category, category) == 0) {
            printf("ID: %d | Title: %s | Author: %s | %s\n", library[i].id,
                library[i].title, library[i].author,
                library[i].isBorrowed ? "Borrowed" : "Available");
        }
    }
}

void listCategories() {
    printf("\nChoose Category:\n1. Fiction\n2. Autobiography\n3. Sports\nEnter choice: ");
    int choice;
    scanf("%d", &choice);
    getchar();
    switch (choice) {
        case 1: viewByCategory("Fiction"); break;
        case 2: viewByCategory("Autobiography"); break;
        case 3: viewByCategory("Sports"); break;
        default: printf("Invalid category.\n");
    }
}

void addBook() {
    if (count >= MAX_BOOKS) {
        printf("Library is full!\n");
        return;
    }

    Book b;
    b.id = count + 1;

    printf("Enter Title: ");
    fgets(b.title, 100, stdin);
    b.title[strcspn(b.title, "\n")] = 0;

    printf("Enter Author: ");
    fgets(b.author, 100, stdin);
    b.author[strcspn(b.author, "\n")] = 0;

    printf("Choose Category:\n1. Fiction\n2. Autobiography\n3. Sports\nChoice: ");
    int cat;
    scanf("%d", &cat);
    getchar();

    switch (cat) {
        case 1: strcpy(b.category, "Fiction"); break;
        case 2: strcpy(b.category, "Autobiography"); break;
        case 3: strcpy(b.category, "Sports"); break;
        default: printf("Invalid category.\n"); return;
    }

    printf("Enter path to book file (e.g., books/title.txt): ");
    fgets(b.filepath, 100, stdin);
    b.filepath[strcspn(b.filepath, "\n")] = 0;

    b.isBorrowed = 0;
    library[count++] = b;
    printf("Book added successfully!\n");
}

void borrowBook() {
    int id;
    printf("Enter Book ID to borrow: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (library[i].id == id) {
            if (library[i].isBorrowed) {
                printf("Book is already borrowed.\n");
                return;
            }
            printf("Enter today's date (dd mm yyyy): ");
            scanf("%d %d %d", &library[i].borrowDay, &library[i].borrowMonth, &library[i].borrowYear);
            library[i].isBorrowed = 1;
            printf("Borrowed successfully. Return in 7 days.\n");
            return;
        }
    }
    printf("Book not found.\n");
}

void returnBook() {
    int id, d, m, y;
    printf("Enter Book ID to return: ");
    scanf("%d", &id);

    for (int i = 0; i < count; i++) {
        if (library[i].id == id && library[i].isBorrowed) {
            printf("Enter return date (dd mm yyyy): ");
            scanf("%d %d %d", &d, &m, &y);

            int diff = dateDifference(library[i].borrowDay, library[i].borrowMonth, library[i].borrowYear, d, m, y);
            if (diff <= 7)
                printf("Returned on time. No fine.\n");
            else
                printf("Late return: ₹%d fine.\n", (diff - 7) * 10);

            library[i].isBorrowed = 0;
            return;
        }
    }
    printf("Book not borrowed or not found.\n");
}

void listBooks() {
    printf("\nAll Books:\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d | Title: %s | Author: %s | %s | %s\n",
               library[i].id, library[i].title, library[i].author,
               library[i].category, library[i].isBorrowed ? "Borrowed" : "Available");
    }
}

void openBook() {
    int id;
    printf("Enter Book ID to open: ");
    scanf("%d", &id);
    getchar();

    for (int i = 0; i < count; i++) {
        if (library[i].id == id) {
            if (library[i].isBorrowed) {
                printf("This book is currently borrowed and unavailable to open.\n");
                return;
            }

            char command[150];
#if defined(_WIN32)
            sprintf(command, "start \"\" \"%s\"", library[i].filepath);
#elif defined(__APPLE__)
            sprintf(command, "open \"%s\"", library[i].filepath);
#else
            sprintf(command, "xdg-open \"%s\"", library[i].filepath);
#endif
            int result = system(command);
            if (result != 0) {
                printf("Failed to open the file.\n");
            }
            return;
        }
    }
    printf("Book not found.\n");
}


int main() {
    preloadBooks();
    int choice;

    do {
        printf("\n--- Library Menu ---\n");
        printf("1. View by Category\n");
        printf("2. Add Book\n");
        printf("3. Borrow Book\n");
        printf("4. Return Book\n");
        printf("5. List All Books\n");
        printf("6. Open Book File\n");
        printf("7. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: listCategories(); break;
            case 2: addBook(); break;
            case 3: borrowBook(); break;
            case 4: returnBook(); break;
            case 5: listBooks(); break;
            case 6: openBook(); break;
            case 7: printf("Goodbye!\n"); break;
            default: printf("Invalid option.\n");
        }
    } while (choice != 7);
}