#include <stdio.h>
#include "contact.h"
// Text color macros
#define RESET   "\033[0m"
#define BLACK   "\033[0;30m"
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE    "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN    "\033[0;36m"
#define WHITE   "\033[0;37m"

int main() {
    int choice;
    AddressBook addressBook;
    initialize(&addressBook); // Initialize the address book

    do 
    {
        printf(BLUE"\n========== ADDRESS BOOK MENU ========\n"RESET);
        printf(MAGENTA"| %-3s | %-25s |\n", "No.", "Options"RESET);
        printf(MAGENTA"-------------------------------------\n"RESET);
        printf(BLUE"| %-3d | %-27s |\n", 1, "Create contact 📝"RESET);
        printf(BLUE"| %-3d | %-27s |\n", 2, "Search contact 🔎"RESET);
        printf(BLUE"| %-3d | %-30s |\n", 3, "Edit contact ✏️"RESET);
        printf(BLUE"| %-3d | %-31s |\n", 4, "Delete contact 🗑️"RESET);
        printf(BLUE"| %-3d | %-27s |\n", 5, "List all contacts 📋"RESET);
        printf(BLUE"| %-3d | %-27s |\n", 6, "Exit 🚪"RESET);
        printf(MAGENTA"======================================\n"RESET);
        printf("Enter your choice between (1 to 6): ");
        scanf("%d", &choice);
        getchar();
        switch (choice) 
        {
            case 1:
                createContact(&addressBook);
                break;
            case 2:
                searchContact(&addressBook);
                break;
            case 3:
                editContact(&addressBook);
                break;
            case 4:
                deleteContact(&addressBook);
                break;
            case 5:
                printf(MAGENTA"\n------ Sort Contacts Menu ------\n"RESET);
                printf(BLUE"| %-3s | %-20s |\n", "No.", "Sort by"RESET);
                printf(MAGENTA"-------------------------------\n"RESET);
                printf(BLUE"| %-3d | %-20s |\n", 1, "Name"RESET);
                printf(BLUE"| %-3d | %-20s |\n", 2, "Email"RESET);
                printf(BLUE"| %-3d | %-20s |\n", 3, "Phone"RESET);
                printf(MAGENTA"-------------------------------\n"RESET);
                printf("Enter your choice (1 to 3): ");
                int sortChoice;
                scanf("%d", &sortChoice);
                getchar();
                if(sortChoice>=1 && sortChoice<=3)
                {
                    listContacts(&addressBook,sortChoice);
                }
                else
                {
                    printf(RED"Enter the choice between 1 to 6 to sort the contacts\n"RESET);
                    break;
                }
                break;
            case 6:
                printf(RED"Saving and Exiting...\n"RESET);
                saveContactsToFile(&addressBook);
                return 0;
                
            default:
                
                printf(RED"Invalid choice. Please try again.\n"RESET);
                break;                
        }

    } while (choice!=6);
    
       return 0;
}
