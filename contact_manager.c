#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define SIZE 10
#define NAME_LEN 50
#define PHONE_LEN 15

struct Contact {
    char name[NAME_LEN];
    char phone[PHONE_LEN];
    struct Contact *next;
};

struct Contact *hashTable[SIZE];

// remove newline from fgets
void trim_newline(char *s) {
    size_t len = strlen(s);
    if (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r'))
        s[len - 1] = '\0';
}

// make string lowercase
void to_lowercase(char *s) {
    for (int i = 0; s[i]; i++)
        s[i] = (char)tolower((unsigned char)s[i]);
}

// hash function: sum of ASCII values % SIZE
int hash(const char *name) {
    int sum = 0;
    for (int i = 0; name[i] != '\0'; i++)
        sum += (unsigned char)name[i];
    return sum % SIZE;
}

struct Contact* createContact(const char *name, const char *phone) {
    struct Contact *p = (struct Contact*)malloc(sizeof(struct Contact));
    strcpy(p->name, name);
    strcpy(p->phone, phone);
    p->next = NULL;
    return p;
}

// add or update contact
void addOrUpdateContact(char *name, char *phone) {
    char lowerName[NAME_LEN];
    strcpy(lowerName, name);
    to_lowercase(lowerName);

    int index = hash(lowerName);
    struct Contact *cur = hashTable[index];

    while (cur) {
        char tempName[NAME_LEN];
        strcpy(tempName, cur->name);
        to_lowercase(tempName);

        if (strcmp(tempName, lowerName) == 0) {
            strcpy(cur->phone, phone);
            printf("\n✅ Contact '%s' updated successfully at index %d.\n", name, index);
            return;
        }
        cur = cur->next;
    }

    struct Contact *newContact = createContact(name, phone);
    newContact->next = hashTable[index];
    hashTable[index] = newContact;
    printf("\n📱 Contact '%s' added successfully at index %d.\n", name, index);
}

// search contact
void searchContact(char *name) {
    char lowerName[NAME_LEN];
    strcpy(lowerName, name);
    to_lowercase(lowerName);

    int index = hash(lowerName);
    struct Contact *cur = hashTable[index];

    while (cur) {
        char tempName[NAME_LEN];
        strcpy(tempName, cur->name);
        to_lowercase(tempName);

        if (strcmp(tempName, lowerName) == 0) {
            printf("\n📞 Contact found!\n");
            printf("+----------------------+----------------+\n");
            printf("| Name                 | Phone          |\n");
            printf("+----------------------+----------------+\n");
            printf("| %-20s | %-14s |\n", cur->name, cur->phone);
            printf("+----------------------+----------------+\n");
            return;
        }
        cur = cur->next;
    }
    printf("\n❌ Contact not found!\n");
}

// delete contact
void deleteContact(char *name) {
    char lowerName[NAME_LEN];
    strcpy(lowerName, name);
    to_lowercase(lowerName);

    int index = hash(lowerName);
    struct Contact *cur = hashTable[index];
    struct Contact *prev = NULL;

    while (cur) {
        char tempName[NAME_LEN];
        strcpy(tempName, cur->name);
        to_lowercase(tempName);

        if (strcmp(tempName, lowerName) == 0) {
            if (prev == NULL)
                hashTable[index] = cur->next;
            else
                prev->next = cur->next;
            free(cur);
            printf("\n🗑️ Contact '%s' deleted successfully.\n", name);
            return;
        }
        prev = cur;
        cur = cur->next;
    }
    printf("\n❌ Contact not found for deletion!\n");
}

// ✅ display all contacts in tabular format
void displayContacts() {
    printf("\n=====================================\n");
    printf("        📘 ALL CONTACTS LIST\n");
    printf("=====================================\n");

    printf("+-------+----------------------+----------------+\n");
    printf("| Index | Name                 | Phone          |\n");
    printf("+-------+----------------------+----------------+\n");

    int empty = 1;
    for (int i = 0; i < SIZE; i++) {
        struct Contact *cur = hashTable[i];
        while (cur) {
            printf("| %-5d | %-20s | %-14s |\n", i, cur->name, cur->phone);
            cur = cur->next;
            empty = 0;
        }
    }

    if (empty)
        printf("|          No contacts found!            |\n");

    printf("+-------+----------------------+----------------+\n");
}

int main() {
    int choice;
    char name[NAME_LEN], phone[PHONE_LEN];

    printf("====================================\n");
    printf("     CONTACT MANAGER USING HASHING  \n");
    printf("====================================\n");

    while (1) {
        printf("\nMenu:\n");
        printf("1. Add/Update Contact\n");
        printf("2. Search Contact\n");
        printf("3. Delete Contact\n");
        printf("4. Display All Contacts\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Try again!\n");
            continue;
        }
        getchar(); // clear newline

        switch (choice) {
            case 1:
                printf("Enter Name: ");
                fgets(name, sizeof(name), stdin);
                trim_newline(name);
                printf("Enter Phone: ");
                fgets(phone, sizeof(phone), stdin);
                trim_newline(phone);
                addOrUpdateContact(name, phone);
                break;

            case 2:
                printf("Enter Name to Search: ");
                fgets(name, sizeof(name), stdin);
                trim_newline(name);
                searchContact(name);
                break;

            case 3:
                printf("Enter Name to Delete: ");
                fgets(name, sizeof(name), stdin);
                trim_newline(name);
                deleteContact(name);
                break;

            case 4:
                displayContacts();
                break;

            case 5:
                printf("\n👋 Exiting Contact Manager. Goodbye!\n");
                exit(0);

            default:
                printf("Invalid choice. Try again!\n");
        }
    }
    return 0;
}
