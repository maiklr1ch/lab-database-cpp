
#include <iostream>

#define FILENAME "dishes.txt"
#define FILENAME_B "dishes.dat"

using namespace std;

enum DishTypes {
	DISH_HOT,
	DISH_COLD,
	DESSERT
};

struct Dish {
	char name[50];
    DishTypes type;
	double price;
};

const char* getTypeLiteral(DishTypes type)
{
    switch (type)
    {
        case DISH_HOT: 
            return "Hot dish";
        case DISH_COLD:
            return "Cold dish";
        case DESSERT:
            return "Dessert";
        default:
            return "Unknown";
    }
}

void addDishes() {
    Dish newDish;

    FILE* fp;
    if (int e = fopen_s(&fp, FILENAME_B, "ab"))
    {
        cout << e<<endl;
        printf("Error while opening file %s", FILENAME_B);
        return;
    }
    fseek(fp, 0, SEEK_END);
    while (true)
    {
        int newType = -1;
        cout << "\n* Enter name: ";
        cin >> newDish.name;
        if (newDish.name[0] == 'E' && !newDish.name[1]) break;
        while (newType < 0 || newType > 2)
        {
            cout << "* Choose type ( 0 - hot, 1 - cold, 2 - dessert ): ";
            cin >> newType;
        }
        cout << "* Enter price: ";
        cin >> newDish.price;
        if (newDish.price < 0) break;
        newDish.type = static_cast<DishTypes>(newType);
        fwrite(&newDish, sizeof(newDish), 1, fp);
        printf(">>> Added %s - %s - %.2f\n>> To exit, enter now 'E'", newDish.name, getTypeLiteral(newDish.type), newDish.price);
    }
    fclose(fp);
}

Dish* getDishes(int& countOut) {
    FILE* fp;
    if (fopen_s(&fp, FILENAME_B, "rb"))
    {
        printf("Error while opening file %s", FILENAME);
        return nullptr;
    }
    fseek(fp, 0, SEEK_END);
    const int lastPosition = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    const int recordsCount = lastPosition / sizeof(Dish);
    if (lastPosition < sizeof(Dish))
    {
        printf("There is no records\n");
        return nullptr;
    }
    Dish* dishList = new Dish[recordsCount];
    fread_s(dishList, sizeof(Dish) * recordsCount, sizeof(Dish), recordsCount, fp);
    fclose(fp);
    countOut = recordsCount;
    return dishList;
}

void menuAll () {
    int recordsCount;
    Dish* dishList = getDishes(recordsCount);
    int counter = 1;
    for (int i = 0; i < recordsCount; i++)
    {
        printf("%d | %s | %s | Price: %.2f\n", counter, dishList[i].name, getTypeLiteral(dishList[i].type), dishList[i].price);
        counter++;
    }
    delete[] dishList;
}

void menuTyped()
{
    int t = -1;
    while (t < 0 || t > 2) {
        printf(">>> Select type ( 0 - hot, 1 - cold, 2 - dessert ): ");
        cin >> t;
    }

    int recordsCount;
    Dish* dishList = getDishes(recordsCount);
    int counter = 1;
    for (int i = 0; i < recordsCount; i++)
    {
        if (t != -1)
        {
            if (dishList[i].type == t)
            {
                printf("Dish %d | Name: %s | Type: %s | Price: %.2f\n", counter, dishList[i].name, getTypeLiteral(dishList[i].type), dishList[i].price);
                counter++;
            }
        }
        else {
            printf("Dish %d | Name: %s | Type: %s | Price: %.2f\n", counter, dishList[i].name, getTypeLiteral(dishList[i].type), dishList[i].price);
            counter++;
        }
    }
    delete[] dishList;
}

void checkout() {
    int recordsCount;
    Dish* dishList = getDishes(recordsCount);

    int* checkout = new int[recordsCount]; // count of every dish
    for (int i = 0; i < recordsCount; i++)
        checkout[i] = 0;
    menuAll();
    while (true)
    {
        int n, count;
        printf(">>> Enter dish number(1-%d): ", recordsCount);
        cin >> n;
        if (n < 1 || n > recordsCount) break;
        printf(">>> Enter quantity (price %.2f): ", dishList[n - 1].price);
        cin >> count;
        printf("\n>>> To end checkout enter 0\n");
        if (count < 1) break;
        *(checkout+n-1) += count;
    }
    double sum = 0;
    printf("================\n");
    for (int i = 0; i < recordsCount; i++) {
        if (checkout[i])
        {
            sum += checkout[i] * dishList[i].price;
            printf("%s - %d portions - Price %.2f (%.2f per one)\n", dishList[i].name, checkout[i], checkout[i] * dishList[i].price, dishList[i].price);
        }
    }
    printf("Total sum: %.2f\n================\n", sum);
}
/*
void deleteDishes() {
    menuAll();
    int idx;
    printf(">>> enter number: ");
    cin >> idx;
    FILE* fp;
    if (fopen_s(&fp, FILENAME_B, "rb"))
    {
        printf("Error while opening file %s", FILENAME);
        return;
    }
    fseek(fp, 0, SEEK_END);
    const int lastPosition = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    const int recordsCount = lastPosition / sizeof(Dish);
    if (recordsCount < idx)
    {
        printf("There is no records\n");
        return;
    }
    Dish* dishList = new Dish[recordsCount];
    fread_s(dishList, sizeof(Dish) * recordsCount, sizeof(Dish), recordsCount, fp);
    for (int i = idx; i < recordsCount-1; i++) {
        d
    }
    
    fseek(fp, 0, SEEK_SET);
    fwrite(dishList, sizeof(Dish), recordsCount, fp);
   fseek(fp, (recordsCount - 1) * sizeof(Dish), SEEK_SET);
    fwrite(, sizeof(Dish), 1, fp);
    fclose(fp);
}*/

void mainMenu() {
    char action = ' ';
    while (action != 'E')
    {
        printf("\n* Please select option:\n> A - Add dishes\n> C - Checkout\n> D - Delete dishes\n> M - Menu (full)\n> T - Menu (by type)\n> E - Exit\n>>> ");
        cin >> action;
        switch (action)
        {
        case 'A':
            printf(">>> Add dish\n");
            addDishes();
            break;
        case 'C':
            printf(">>> Make checkout\n");
            checkout();
            break;
        case 'D':
            printf(">>> Delete dishes\n");
            //deleteDishes();
            break;
        case 'M':
            printf(">>> Menu (full)\n");
            menuAll();
            break;
        case 'T':
            printf(">>> Menu (by type)\n");
            menuTyped();
            break;
        case 'E':
            printf(">>> Exit\n");
            break;
        default:
            printf(">>> Incorrect option. Try again\n");
            break;
        }
    }
}

int main() {
    mainMenu();
    return 0;
}

