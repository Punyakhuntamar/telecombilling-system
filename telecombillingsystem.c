#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>  // For gotoxy() and getch()

#define MAX_CUSTOMERS 100
#define MAX_BILLINGS 100
#define PHONE_LENGTH 11
#define PASSWORD_FILE "password.txt"  // File to store the password
#define WIDTH 80   // Width of the screen (console)
#define HEIGHT 25  // Height of the screen (console)

typedef struct {
    int id;
    char name[100];
    char phone[PHONE_LENGTH];
    char address[200];
    char plan[10];  // "prepaid" or "postpaid"
} Customer;

typedef struct {
    int customerId;
    float callDuration;  // in minutes
    float internetData;  // in MB
    float roamingCalls;  // in minutes
    float internationalCalls;  // in minutes
} Billing;

Customer customers[MAX_CUSTOMERS];
Billing billings[MAX_BILLINGS];
int customerCount = 0, billingCount = 0;

const char* customerFile = "customers.txt";
const char* billingFile = "billing.txt";


// Function declarations

void loadCustomerData();
void loadBillingData();
void saveCustomerData();
void saveBillingData();
void addCustomer();
void updateCustomer();
void deleteCustomer();
void viewAllCustomers();
void viewSingleCustomer();
void addBillingDetails();
void updateBillingDetails();
void viewAllBillingDetails();
void generateBill();
void viewBillingForSingleCustomer();
void exits();
void menu();


// Function prototypes

int check_password(const char *input_password);
void change_password();


// Function to check if the entered password is correct

int check_password(const char *input_password) {
    char stored_password[20];
    FILE *file = fopen(PASSWORD_FILE, "r");

    if (file == NULL) {
        // If the file does not exist, prompt to set a new password
        printf("No password set! Setting a new password...\n");
        printf("\n");
        printf("Enter a new password: ");
        fgets(stored_password, sizeof(stored_password), stdin);
        stored_password[strcspn(stored_password, "\n")] = '\0';  // Remove newline

        file = fopen(PASSWORD_FILE, "w");
        if (file != NULL) {
            fputs(stored_password, file);  // Save the password
            fclose(file);
            printf("\n");
            printf("Password set successfully!\n");
        }
        return 1;  // Return true, as the password is now set
    }

    // Read the stored password from the file
    fgets(stored_password, sizeof(stored_password), file);
    stored_password[strcspn(stored_password, "\n")] = '\0';  // Remove newline
    fclose(file);

    // Compare the entered password with the stored one
    return strcmp(input_password, stored_password) == 0;
}


// Function to change the stored password

void change_password() {
    char new_password[20], confirm_password[20];
    printf("\n");
    printf("Enter the new password: ");
    fgets(new_password, sizeof(new_password), stdin);
    new_password[strcspn(new_password, "\n")] = '\0';  // Remove newline character

    printf("\n");
    printf("Confirm the new password: ");
    fgets(confirm_password, sizeof(confirm_password), stdin);
    confirm_password[strcspn(confirm_password, "\n")] = '\0';  // Remove newline character

    // Check if passwords match
    if (strcmp(new_password, confirm_password) == 0) {
        FILE *file = fopen(PASSWORD_FILE, "w");
        if (file != NULL) {
            fputs(new_password, file);  // Save the new password
            fclose(file);
            printf("\n");
            printf("Password changed successfully!\n");
        } else {
            printf("Error saving the new password.\n");
        }
    } else {
        printf("Passwords do not match! Try again.\n");
    }
}

// Function to print text at a specific position

void gotoxy(int x, int y) {
    printf("\n");
    printf("\033[%d;%dH", y, x);
}


// Load customer data from the file

void loadCustomerData() {
    FILE *file = fopen(customerFile, "r");
    if (!file) return;
    while (fscanf(file, "%d %s %s %[^\n]s %s", &customers[customerCount].id, customers[customerCount].name, customers[customerCount].phone, customers[customerCount].address, customers[customerCount].plan) != EOF) {
        customerCount++;
    }
    fclose(file);
}


// Load billing data from the file

void loadBillingData() {
    FILE *file = fopen(billingFile, "r");
    if (!file) return;
    while (fscanf(file, "%d %f %f %f %f", &billings[billingCount].customerId, &billings[billingCount].callDuration, &billings[billingCount].internetData, &billings[billingCount].roamingCalls, &billings[billingCount].internationalCalls) != EOF) {
        billingCount++;
    }
    fclose(file);
}


// Save customer data to the file

void saveCustomerData() {
    FILE *file = fopen(customerFile, "a");
    for (int i = 0; i < customerCount; i++) {
        printf("\n");
        fprintf(file, "%d %s %s %s %s\n", customers[i].id, customers[i].name, customers[i].phone, customers[i].address, customers[i].plan);
    }
    fclose(file);
}


// Save billing data to the file

void saveBillingData() {
    FILE *file = fopen(billingFile, "a");
    for (int i = 0; i < billingCount; i++) {
        printf("\n");
        fprintf(file, "%d %.2f %.2f %.2f %.2f\n", billings[i].customerId, billings[i].callDuration, billings[i].internetData, billings[i].roamingCalls, billings[i].internationalCalls);
    }
    fclose(file);
}


// Validate if phone number is 10 digits

int isValidPhoneNumber(char phone[]) {
    if (strlen(phone) != 10) return 0;
    for (int i = 0; i < 10; i++) {
        if (phone[i] < '0' || phone[i] > '9') {
            return 0;
        }
    }
    return 1;
}

void exits(){

    // Topic to display
    char topic[] = "THANKS FOR VISITING";

    // Calculate the position to center the text
    int x = (WIDTH - strlen(topic)) / 2;  // Horizontal center
    int y = HEIGHT / 2;                   // Vertical center

    // Clear the screen (optional)
    printf("\033[2J");  // ANSI escape code to clear screen

    // Display the topic in the center
    gotoxy(x, y);
    printf("%s", topic);

    // Prompt user to press any key to advance
    gotoxy(HEIGHT - 2, WIDTH / 2 - 15);
    printf("\n");
    printf("Press any key to continue...");

    // Wait for a key press before advancing
    getch();  // Wait for user to press any key

    // Move to the next part of the program (you can add more logic here)
    printf("\n");
    printf("\n\nYou pressed a key! Continuing...\n");
             exit(0);
}

// Add a new customer

void addCustomer() {

    system("cls");

    // Topic to display
    char topic[] = "ADDING CUSTOMER DETAILS";

    // Calculate the position to center the text
    int x = (WIDTH - strlen(topic)) / 2;  // Horizontal center
    int y = HEIGHT / 2;                   // Vertical center

    // Clear the screen (optional)
    printf("\033[2J");  // ANSI escape code to clear screen

    // Display the topic in the center
    gotoxy(x, y);
    printf("%s", topic);

    // Prompt user to press any key to advance
    gotoxy(HEIGHT - 2, WIDTH / 2 - 15);
    printf("\n");
    printf("Press any key to continue...");

    // Wait for a key press before advancing
    getch();  // Wait for user to press any key

    // Move to the next part of the program (you can add more logic here)
    printf("\n");
    printf("\n\nYou pressed a key! Continuing...\n");

    system("cls");

    Customer newCustomer;
    printf("\n");
    printf("Enter Customer ID: ");
    scanf("%d", &newCustomer.id);

    // Check if the ID already exists
    for (int i = 0; i < customerCount; i++) {
        if (customers[i].id == newCustomer.id) {
            printf("\n");
            printf("Customer ID already exists!\n");
            return;
        }
    }

    printf("\n");
    printf("Enter Name: ");
    scanf("%s", newCustomer.name);

    // Phone number validation
    while (1) {
        printf("\n");
        printf("Enter Phone Number (10 digits): ");
        scanf("%s", newCustomer.phone);
        if (isValidPhoneNumber(newCustomer.phone)) break;
        else printf("Invalid phone number. Please enter again.\n");
    }

    printf("\n");
    printf("Enter Address: ");
    getchar(); // Consume newline character left by previous scanf
    fgets(newCustomer.address, 200, stdin);
    newCustomer.address[strcspn(newCustomer.address, "\n")] = 0; // Remove trailing newline

    // Plan type validation
    while (1) {
        printf("\n");
        printf("Enter Plan Type (prepaid/postpaid): ");
        scanf("%s", newCustomer.plan);
        if (strcmp(newCustomer.plan, "prepaid") == 0 || strcmp(newCustomer.plan, "postpaid") == 0) break;
        else printf("Invalid plan type. Please enter again.\n");
    }

    customers[customerCount++] = newCustomer;
    saveCustomerData();
    printf("\n");
    printf("Customer added successfully.\n");
}


// Update customer details

void updateCustomer() {

    system("cls");

    // Topic to display
    char topic[] = "UPDATING CUSTOMER DETAILS";

    // Calculate the position to center the text
    int x = (WIDTH - strlen(topic)) / 2;  // Horizontal center
    int y = HEIGHT / 2;                   // Vertical center

    // Clear the screen (optional)
    printf("\033[2J");  // ANSI escape code to clear screen

    // Display the topic in the center
    gotoxy(x, y);
    printf("%s", topic);

    // Prompt user to press any key to advance
    gotoxy(HEIGHT - 2, WIDTH / 2 - 15);
    printf("\n");
    printf("Press any key to continue...");

    // Wait for a key press before advancing
    getch();  // Wait for user to press any key

    // Move to the next part of the program (you can add more logic here)
    printf("\n");
    printf("\n\nYou pressed a key! Continuing...\n");

    system("cls");

    int id, attempts = 3;
    printf("\n");
    printf("Enter Customer ID to update: ");
    scanf("%d", &id);

    // Find the customer
    int index = -1;
    for (int i = 0; i < customerCount; i++) {
        if (customers[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("\n");
        printf("Customer ID not found.\n");
        return;
    }

    char choice[3];
    printf("\n");
    printf("Do you want to update Name (yes/no)? ");
    scanf("%s", choice);
    if (strcmp(choice, "yes") == 0) {
        printf("\n");
        printf("Enter new Name: ");
        scanf("%s", customers[index].name);
    }

    printf("\n");
    printf("Do you want to update Phone Number (yes/no)? ");
    scanf("%s", choice);
    if (strcmp(choice, "yes") == 0) {
        char newPhone[PHONE_LENGTH];
        while (1) {
            printf("\n");
            printf("Enter new Phone Number (10 digits): ");
            scanf("%s", newPhone);
            if (isValidPhoneNumber(newPhone)) {
                strcpy(customers[index].phone, newPhone);
                break;
            } else {
                printf("\n");
                printf("Invalid phone number. Please enter again.\n");
            }
        }
    }

    printf("\n");
    printf("Do you want to update Address (yes/no)? ");
    scanf("%s", choice);
    if (strcmp(choice, "yes") == 0) {
        printf("\n");
        printf("Enter new Address: ");
        getchar(); // Consume newline character left by previous scanf
        fgets(customers[index].address, 200, stdin);
        customers[index].address[strcspn(customers[index].address, "\n")] = 0; // Remove trailing newline
    }

    printf("\n");
    printf("Do you want to update Plan Type (yes/no)? ");
    scanf("%s", choice);
    if (strcmp(choice, "yes") == 0) {
        while (1) {
            printf("\n");
            printf("Enter new Plan Type (prepaid/postpaid): ");
            scanf("%s", customers[index].plan);
            if (strcmp(customers[index].plan, "prepaid") == 0 || strcmp(customers[index].plan, "postpaid") == 0) {
                break;
            } else {
                printf("\n");
                printf("Invalid plan type. Please enter again.\n");
            }
        }
    }

    saveCustomerData();
    printf("\n");
    printf("Customer updated successfully.\n");
}


// Delete customer details

void deleteCustomer() {

    system("cls");

    // Topic to display
    char topic[] = "DELETING CUSTOMER DETAILS";

    // Calculate the position to center the text
    int x = (WIDTH - strlen(topic)) / 2;  // Horizontal center
    int y = HEIGHT / 2;                   // Vertical center

    // Clear the screen (optional)
    printf("\033[2J");  // ANSI escape code to clear screen

    // Display the topic in the center
    gotoxy(x, y);
    printf("%s", topic);

    // Prompt user to press any key to advance
    gotoxy(HEIGHT - 2, WIDTH / 2 - 15);
    printf("\n");
    printf("Press any key to continue...");

    // Wait for a key press before advancing
    getch();  // Wait for user to press any key

    // Move to the next part of the program (you can add more logic here)
    printf("\n");
    printf("\n\nYou pressed a key! Continuing...\n");

    system("cls");

    int id;
    printf("\n");
    printf("Enter Customer ID to delete: ");
    scanf("%d", &id);

    int index = -1;
    for (int i = 0; i < customerCount; i++) {
        if (customers[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("\n");
        printf("Customer ID not found.\n");
        return;
    }

    char choice[3];
    printf("\n");
    printf("Are you sure you want to delete this customer (yes/no)? ");
    scanf("%s", choice);
    if (strcmp(choice, "yes") == 0) {
        for (int i = index; i < customerCount - 1; i++) {
            customers[i] = customers[i + 1];
        }
        customerCount--;
        saveCustomerData();
        printf("\n");
        printf("Customer deleted successfully.\n");
    } else {
        printf("Deletion canceled.\n");
    }
}


// View all customer details

void viewAllCustomers() {

    system("cls");

    // Topic to display
    char topic[] = "VIEWING ALL CUSTOMER'S DETAILS";

    // Calculate the position to center the text
    int x = (WIDTH - strlen(topic)) / 2;  // Horizontal center
    int y = HEIGHT / 2;                   // Vertical center

    // Clear the screen (optional)
    printf("\033[2J");  // ANSI escape code to clear screen

    // Display the topic in the center
    gotoxy(x, y);
    printf("%s", topic);

    // Prompt user to press any key to advance
    gotoxy(HEIGHT - 2, WIDTH / 2 - 15);
    printf("\n");
    printf("Press any key to continue...");

    // Wait for a key press before advancing
    getch();  // Wait for user to press any key

    // Move to the next part of the program (you can add more logic here)
    printf("\n");
    printf("\n\nYou pressed a key! Continuing...\n");

    system("cls");

    printf("\n");
    printf("\nCustomer ID\t\tName\t\tPhone\t\tAddress\t\tPlan Type\n");
    for (int i = 0; i < customerCount; i++) {
        printf("\n");
        printf("%d\t\t\t%s\t\t%s\t%s\t\t%s\n", customers[i].id, customers[i].name, customers[i].phone, customers[i].address, customers[i].plan);
    }
}


// View a single customer's details

void viewSingleCustomer() {

    system("cls");

    // Topic to display
    char topic[] = "VIEWING SINGLE CUSTOMER DETAILS";

    // Calculate the position to center the text
    int x = (WIDTH - strlen(topic)) / 2;  // Horizontal center
    int y = HEIGHT / 2;                   // Vertical center

    // Clear the screen (optional)
    printf("\033[2J");  // ANSI escape code to clear screen

    // Display the topic in the center
    gotoxy(x, y);
    printf("%s", topic);

    // Prompt user to press any key to advance
    gotoxy(HEIGHT - 2, WIDTH / 2 - 15);
    printf("\n");
    printf("Press any key to continue...");

    // Wait for a key press before advancing
    getch();  // Wait for user to press any key

    // Move to the next part of the program (you can add more logic here)
    printf("\n");
    printf("\n\nYou pressed a key! Continuing...\n");

    system("cls");

    int id, attempts = 3;
    printf("\n");
    printf("Enter Customer ID to view: ");
    scanf("%d", &id);

    int index = -1;
    for (int i = 0; i < customerCount; i++) {
        if (customers[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("\n");
        printf("Customer ID not found.\n");
        return;
    }

    printf("\n");
    printf("\nCustomer ID: %d\nName: %s\nPhone: %s\nAddress: %s\nPlan Type: %s\n", customers[index].id, customers[index].name, customers[index].phone, customers[index].address, customers[index].plan);
}


// Function to add billing details for an existing customer

void addBillingDetails() {

    system("cls");

    // Topic to display
    char topic[] = "ADDING BILLING DETAILS";

    // Calculate the position to center the text
    int x = (WIDTH - strlen(topic)) / 2;  // Horizontal center
    int y = HEIGHT / 2;                   // Vertical center

    // Clear the screen (optional)
    printf("\033[2J");  // ANSI escape code to clear screen

    // Display the topic in the center
    gotoxy(x, y);
    printf("%s", topic);

    // Prompt user to press any key to advance
    gotoxy(HEIGHT - 2, WIDTH / 2 - 15);
    printf("\n");
    printf("Press any key to continue...");

    // Wait for a key press before advancing
    getch();  // Wait for user to press any key

    // Move to the next part of the program (you can add more logic here)
    printf("\n");
    printf("\n\nYou pressed a key! Continuing...\n");

    system("cls");

    int id, attempts = 3;
    printf("\n");
    printf("Enter Customer ID to add billing details: ");
    scanf("%d", &id);

    // Find the customer
    int index = -1;
    for (int i = 0; i < customerCount; i++) {
        if (customers[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("\n");
        printf("Customer ID not found.\n");
        return;
    }

    // Check if billing details already exist for this customer
    for (int i = 0; i < billingCount; i++) {
        if (billings[i].customerId == id) {
            printf("\n");
            printf("Billing details already exist for this customer.\n");
            return;
        }
    }

    Billing newBilling;
    newBilling.customerId = id;
    printf("\n");
    printf("Enter call duration (in minutes): ");
    scanf("%f", &newBilling.callDuration);

    printf("\n");
    printf("Enter internet data usage (in MB): ");
    scanf("%f", &newBilling.internetData);

    printf("\n");
    printf("Enter roaming calls duration (in minutes): ");
    scanf("%f", &newBilling.roamingCalls);

    printf("\n");
    printf("Enter international calls duration (in minutes): ");
    scanf("%f", &newBilling.internationalCalls);

    billings[billingCount++] = newBilling;
    saveBillingData();
    printf("\n");
    printf("Billing details added successfully.\n");
}


// Function to update billing details for an existing customer

void updateBillingDetails() {

    system("cls");

    // Topic to display
    char topic[] = "UPDATING BILLING DETAILS";

    // Calculate the position to center the text
    int x = (WIDTH - strlen(topic)) / 2;  // Horizontal center
    int y = HEIGHT / 2;                   // Vertical center

    // Clear the screen (optional)
    printf("\033[2J");  // ANSI escape code to clear screen

    // Display the topic in the center
    gotoxy(x, y);
    printf("%s", topic);

    // Prompt user to press any key to advance
    gotoxy(HEIGHT - 2, WIDTH / 2 - 15);
    printf("\n");
    printf("Press any key to continue...");

    // Wait for a key press before advancing
    getch();  // Wait for user to press any key

    // Move to the next part of the program (you can add more logic here)
    printf("\n");
    printf("\n\nYou pressed a key! Continuing...\n");

    system("cls");

    int id, attempts = 3;
    printf("\n");
    printf("Enter Customer ID to update billing details: ");
    scanf("%d", &id);

    // Find the billing details
    int index = -1;
    for (int i = 0; i < billingCount; i++) {
        if (billings[i].customerId == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("\n");
        printf("Billing details not found for this customer.\n");
        return;
    }

    char choice[3];
    printf("\n");
    printf("Do you want to update Call Duration (yes/no)? ");
    scanf("%s", choice);
    if (strcmp(choice, "yes") == 0) {
        printf("\n");
        printf("Enter new Call Duration (in minutes): ");
        scanf("%f", &billings[index].callDuration);
    }

    printf("\n");
    printf("Do you want to update Internet Data Usage (yes/no)? ");
    scanf("%s", choice);
    if (strcmp(choice, "yes") == 0) {
        printf("\n");
        printf("Enter new Internet Data Usage (in MB): ");
        scanf("%f", &billings[index].internetData);
    }

    printf("\n");
    printf("Do you want to update Roaming Calls Duration (yes/no)? ");
    scanf("%s", choice);
    if (strcmp(choice, "yes") == 0) {
        printf("\n");
        printf("Enter new Roaming Calls Duration (in minutes): ");
        scanf("%f", &billings[index].roamingCalls);
    }

    printf("\n");
    printf("Do you want to update International Calls Duration (yes/no)? ");
    scanf("%s", choice);
    if (strcmp(choice, "yes") == 0) {
        printf("\n");
        printf("Enter new International Calls Duration (in minutes): ");
        scanf("%f", &billings[index].internationalCalls);
    }

    saveBillingData();
    printf("\n");
    printf("Billing details updated successfully.\n");
}


// Function to view billing details for all customers

void viewAllBillingDetails() {

    system("cls");

    // Topic to display
    char topic[] = "VIEWING ALL CUSTOMER'S BILLING DETAILS";

    // Calculate the position to center the text
    int x = (WIDTH - strlen(topic)) / 2;  // Horizontal center
    int y = HEIGHT / 2;                   // Vertical center

    // Clear the screen (optional)
    printf("\033[2J");  // ANSI escape code to clear screen

    // Display the topic in the center
    gotoxy(x, y);
    printf("%s", topic);

    // Prompt user to press any key to advance
    gotoxy(HEIGHT - 2, WIDTH / 2 - 15);
    printf("\n");
    printf("Press any key to continue...");

    // Wait for a key press before advancing
    getch();  // Wait for user to press any key

    // Move to the next part of the program (you can add more logic here)
    printf("\n");
    printf("\n\nYou pressed a key! Continuing...\n");

    system("cls");

    printf("\n");
    printf("\nCustomer ID\tName\tPhone\t\tCall Duration\tInternet Data\tRoaming Calls\tInternational Calls\n");
    for (int i = 0; i < billingCount; i++) {
        int customerId = billings[i].customerId;
        int customerIndex = -1;
        for (int j = 0; j < customerCount; j++) {
            if (customers[j].id == customerId) {
                customerIndex = j;
                break;
            }
        }

        if (customerIndex != -1) {
            printf("\n");
            printf("%d\t%s\t%s\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\n",
                   customers[customerIndex].id, customers[customerIndex].name, customers[customerIndex].phone,
                   billings[i].callDuration, billings[i].internetData, billings[i].roamingCalls, billings[i].internationalCalls);
        }
    }
}


// Function to generate the bill for a specific customer

void generateBill() {

    system("cls");

    // Topic to display
    char topic[] = "GENERATING BILL";

    // Calculate the position to center the text
    int x = (WIDTH - strlen(topic)) / 2;  // Horizontal center
    int y = HEIGHT / 2;                   // Vertical center

    // Clear the screen (optional)
    printf("\033[2J");  // ANSI escape code to clear screen

    // Display the topic in the center
    gotoxy(x, y);
    printf("%s", topic);

    // Prompt user to press any key to advance
    gotoxy(HEIGHT - 2, WIDTH / 2 - 15);
    printf("\n");
    printf("Press any key to continue...");

    // Wait for a key press before advancing
    getch();  // Wait for user to press any key

    // Move to the next part of the program (you can add more logic here)
    printf("\n");
    printf("\n\nYou pressed a key! Continuing...\n");

    system("cls");

    int id, attempts = 3;
    printf("\n");
    printf("Enter Customer ID to generate the bill: ");
    scanf("%d", &id);

    // Find the billing details for this customer
    int billingIndex = -1;
    for (int i = 0; i < billingCount; i++) {
        if (billings[i].customerId == id) {
            billingIndex = i;
            break;
        }
    }

    if (billingIndex == -1) {
        printf("\n");
        printf("Billing details not found for this customer.\n");
        return;
    }

    int customerIndex = -1;
    for (int i = 0; i < customerCount; i++) {
        if (customers[i].id == id) {
            customerIndex = i;
            break;
        }
    }

    if (customerIndex == -1) {
        printf("\n");
        printf("Customer details not found.\n");
        return;
    }

    // Calculate the bill
    float callCost = billings[billingIndex].callDuration * 1.5;  // 1.5 rupees per minute
    float dataCost = billings[billingIndex].internetData * 5;  // 5 rupees per MB
    float roamingCost = billings[billingIndex].roamingCalls * 3;  // 3 rupees per minute
    float internationalCost = billings[billingIndex].internationalCalls * 6;  // 6 rupees per minute

    float totalCost = callCost + dataCost + roamingCost + internationalCost;

    // Display the bill
    printf("\n");
    printf("\nBill for Customer ID: %d\n", customers[customerIndex].id);
    printf("\n");
    printf("Customer Name: %s\n", customers[customerIndex].name);
    printf("\n");
    printf("Phone: %s\n", customers[customerIndex].phone);
    printf("\n");
    printf("Call Duration: %.2f minutes (Cost: %.2f)\n", billings[billingIndex].callDuration, callCost);
    printf("\n");
    printf("Internet Data: %.2f MB (Cost: %.2f)\n", billings[billingIndex].internetData, dataCost);
    printf("\n");
    printf("Roaming Calls: %.2f minutes (Cost: %.2f)\n", billings[billingIndex].roamingCalls, roamingCost);
    printf("\n");
    printf("International Calls: %.2f minutes (Cost: %.2f)\n", billings[billingIndex].internationalCalls, internationalCost);
    printf("\n");
    printf("\nTotal Bill: %.2f\n", totalCost);
}


// Function to view billing details for a single customer

void viewBillingForSingleCustomer() {

    system("cls");

    // Topic to display
    char topic[] = "VIEWING SINGLE CUSTOMER DETAILS";

    // Calculate the position to center the text
    int x = (WIDTH - strlen(topic)) / 2;  // Horizontal center
    int y = HEIGHT / 2;                   // Vertical center

    // Clear the screen (optional)
    printf("\033[2J");  // ANSI escape code to clear screen

    // Display the topic in the center
    gotoxy(x, y);
    printf("%s", topic);

    // Prompt user to press any key to advance
    gotoxy(HEIGHT - 2, WIDTH / 2 - 15);
    printf("\n");
    printf("Press any key to continue...");

    // Wait for a key press before advancing
    getch();  // Wait for user to press any key

    // Move to the next part of the program (you can add more logic here)
    printf("\n");
    printf("\n\nYou pressed a key! Continuing...\n");

    system("cls");

    int id, attempts = 3;
    printf("\n");
    printf("Enter Customer ID to view billing details: ");
    scanf("%d", &id);

    // Find the billing details for this customer
    int billingIndex = -1;
    for (int i = 0; i < billingCount; i++) {
        if (billings[i].customerId == id) {
            billingIndex = i;
            break;
        }
    }

    if (billingIndex == -1) {
        printf("\n");
        printf("Billing details not found for this customer.\n");
        return;
    }

    int customerIndex = -1;
    for (int i = 0; i < customerCount; i++) {
        if (customers[i].id == id) {
            customerIndex = i;
            break;
        }
    }

    if (customerIndex == -1) {
        printf("\n");
        printf("Customer details not found.\n");
        return;
    }

    // Display the billing details
    printf("\n");
    printf("\nBilling Details for Customer ID: %d\n", customers[customerIndex].id);
    printf("\n");
    printf("Customer Name: %s\n", customers[customerIndex].name);
    printf("\n");
    printf("Phone: %s\n", customers[customerIndex].phone);
    printf("\n");
    printf("Call Duration: %.2f minutes\n", billings[billingIndex].callDuration);
    printf("\n");
    printf("Internet Data: %.2f MB\n", billings[billingIndex].internetData);
    printf("\n");
    printf("Roaming Calls: %.2f minutes\n", billings[billingIndex].roamingCalls);
    printf("\n");
    printf("International Calls: %.2f minutes\n", billings[billingIndex].internationalCalls);
}


// Main menu for navigation

void menu() {
    int choice;
    while (1) {
        printf("\n");
        printf("\nTelecom Billing System Menu:\n");
        printf("\n");
        printf("1. Add Customer\n");
        printf("\n");
        printf("2. Update Customer\n");
        printf("\n");
        printf("3. Delete Customer\n");
        printf("\n");
        printf("4. View All Customers\n");
        printf("\n");
        printf("5. View Single Customer\n");
        printf("\n");
        printf("6. Add Billing Details\n");
        printf("\n");
        printf("7. Update Billing Details\n");
        printf("\n");
        printf("8. View All Billing Details\n");
        printf("\n");
        printf("9. View Billing for Single Customer\n");
        printf("\n");
        printf("10. Generate Bill\n");
        printf("\n");
        printf("11. Exit\n");
        printf("\n");
        printf("Enter your choice: ");
        printf("\n");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addCustomer(); break;
            case 2: updateCustomer(); break;
            case 3: deleteCustomer(); break;
            case 4: viewAllCustomers(); break;
            case 5: viewSingleCustomer(); break;
            case 6: addBillingDetails(); break;
            case 7: updateBillingDetails(); break;
            case 8: viewAllBillingDetails(); break;
            case 9: viewBillingForSingleCustomer(); break;
            case 10: generateBill(); break;
            case 11: exits();
            default: printf("Invalid choice. Try again.\n");
        }
    }
}


int main() {

    // Topic to display
    char topic[] = "TELECOM BILLING SYSTEM";

    // Calculate the position to center the text
    int x = (WIDTH - strlen(topic)) / 2;  // Horizontal center
    int y = HEIGHT / 2;                   // Vertical center

    // Clear the screen (optional)
    printf("\033[2J");  // ANSI escape code to clear screen

    // Display the topic in the center
    gotoxy(x, y);
    printf("%s", topic);

    // Prompt user to press any key to advance
    gotoxy(HEIGHT - 2, WIDTH / 2 - 15);
    printf("\n");
    printf("Press any key to continue...");

    // Wait for a key press before advancing
    getch();  // Wait for user to press any key

    // Move to the next part of the program (you can add more logic here)
    printf("\n");
    printf("\n\nYou pressed a key! Continuing...\n");

    system("cls");

    char entered_password[20];
    int attempts = 3;  // Max 3 attempts to enter the password correctly

    // Ask for password to proceed
    printf("\n");
    printf("Enter the password to access the program: ");

    // Load the stored password from the file and check
    while (attempts > 0) {
        fgets(entered_password, sizeof(entered_password), stdin);
        entered_password[strcspn(entered_password, "\n")] = '\0';  // Remove newline character

        // Check if the entered password is correct
        if (check_password(entered_password)) {
            printf("\n");
            printf("Access granted!\n");
            break;
        } else {
            attempts--;
            printf("\n");
            printf("Incorrect password! You have %d attempt(s) left.\n", attempts);
            if (attempts == 0) {
                printf("\n");
                printf("Access denied.\n");
                return 0;  // Exit program after 3 failed attempts
            }
        }
    }

    system("cls");

    // Menu after successful password entry
    int choice;
    while (1) {
        printf("\nMenu:\n");
        printf("1. Change Password\n");
        printf("2. Perform Task\n");
        printf("3. Exit\n");
        printf("\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // To consume the newline character left by scanf

        switch (choice) {
            case 1:
                system("cls");
                change_password();
                break;
            case 2:
                system("cls");
                printf("\n");
                printf("Performing task...\n");
                menu();
            case 3:
                system("cls");
                printf("\n");
                printf("Exiting the program.\n");
                return 0;
            default:
                printf("\n");
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

