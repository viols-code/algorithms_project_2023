#include <stdio.h>
#include <stdlib.h>

// A struct for the vehicle
typedef struct vehicle{
    // Vehicle's autonomy
    int autonomy;
    // Pointer to the next vehicle
    struct vehicle * next;
} vehicle;

// A struct for the station
typedef struct station{
    // Station's position (= distance from the starting point of the highway)
    int pos;
    // Vehicles' list
    vehicle * vehicles;
    // The maximum autonomy in the station
    int max_autonomy;
    // Pointer to the next station
    struct station * next;
    // Pointer to the previous station
    struct station * prev;
} station;

// A struct for the highway
typedef struct{
    // First station in the highway
    station * start;
    // Last station in the highway
    station * end;
} highway;

// A node for a balanced binary tree
typedef struct node {
    // Pointer to the station
    station * key;
    // Pointer to the left children
    struct node * left;
    // Pointer to the right children
    struct node * right;
    // Height of the node
    int height;
} node;

// A function to compute the maximum
int max(int a, int b) {
    return (a > b) ? a : b;
}

// A function to create a new node
node * newNode(station * key) {
    node * n = (node *) malloc(sizeof(node));
    n->key = key;
    n->left = NULL;
    n->right = NULL;
    n->height = 1;
    return n;
}

// A function to get height of the tree
int height(node * n) {
    if (n == NULL)
        return 0;
    return n->height;
}

// A function to right rotate subtree rooted with y
node * rightRotate(node * n) {
    node * new_root = n->left;
    node * x = new_root->right;

    // Perform rotation
    new_root->right = n;
    n->left = x;

    // Update heights
    n->height = max(height(n->left), height(n->right)) + 1;
    new_root->height = max(height(new_root->left), height(new_root->right)) + 1;

    // Return new root
    return new_root;
}

// A function to left rotate subtree rooted with x
node * leftRotate(node * n) {
    node * new_root = n->right;
    node * x = new_root->left;

    // Perform rotation
    new_root->left = n;
    n->right = x;

    //  Update heights
    n->height = max(height(n->left), height(n->right)) + 1;
    new_root->height = max(height(new_root->left), height(new_root->right)) + 1;

    // Return new root
    return new_root;
}

// Get balance factor
int getBalance(node * n) {
    if (n == NULL)
        return 0;
    return height(n->left) - height(n->right);
}

// A function to insert a node in AVL tree
node * insertNode(node * n, station * key) {
    if (n == NULL) // If the tree is empty
        return (newNode(key));

    if (key->pos < n->key->pos)
        n->left = insertNode(n->left, key);
    else
        n->right = insertNode(n->right, key);

    // Update height of this ancestor node
    n->height = max(height(n->left), height(n->right)) + 1;
    int balance = getBalance(n);

    // If this node becomes unbalanced
    if (balance > 1 && key->pos < n->left->key->pos)
        return rightRotate(n);

    if (balance < -1 && key->pos > n->right->key->pos)
        return leftRotate(n);

    if (balance > 1 && key->pos > n->left->key->pos) {
        n->left = leftRotate(n->left);
        return rightRotate(n);
    }

    if (balance < -1 && key->pos < n->right->key->pos) {
        n->right = rightRotate(n->right);
        return leftRotate(n);
    }

    return n;
}

// Function to create a new vehicle
vehicle * newVehicle(int autonomy){
    vehicle * v = (vehicle *) malloc(sizeof(vehicle));
    v->autonomy = autonomy;
    v->next = NULL;
    return v;
}

// Function to add a vehicle, only distinct autonomy are saved
void insertVehicle(station * s, int autonomy){
    vehicle * vehicles = s->vehicles;
    s->vehicles = newVehicle(autonomy);
    s->vehicles->next = vehicles;
    // Updates the maximum autonomy if necessary
    if(autonomy > s->max_autonomy){
        s->max_autonomy = autonomy;
    }
}

// A function to create a new station
station * newStation() {
    station * s = (station *) malloc(sizeof(station));
    s->vehicles = NULL;
    s->max_autonomy = 0;
    s->next = NULL;
    s->prev = NULL;
    return s;
}

// A function to find the station or the nearest station
station * findStation(node * root, int pos){
    if (root == NULL)
        return NULL;
    if (root->key->pos == pos) // If the position is found
        return root->key;
    if (root->key->pos > pos){ // If the position searched is smaller
        if(root->left == NULL)
            return root->key;
        return findStation(root->left, pos);
    }
    // If the position searched is greater
    if(root->right == NULL)
        return root->key;
    return findStation(root->right, pos);
}

// A function to add the vehicle at the given station
void addVehicle(node * root, char * command){
    char * ns;
    int dist = (int) strtol(command, &ns, 10);
    int autonomy = (int) strtol(ns + 1, &ns, 10);
    station * s = findStation(root, dist); // Find the given station
    if (s == NULL || s->pos != dist){ // If the station is not present
        puts("non aggiunta");
        return;
    }

    insertVehicle(s, autonomy); // Insert the vehicle in the vehicles list
    puts("aggiunta");
}

// A function to insert a station in the linked list
void insertStation(highway * hw, station * s, station * n){
    if(hw->start == NULL){ // The highway is empty
        hw->start = s;
        hw->end = s;
    } else{ // Adding a new station
        station * first_station = hw->start;
        if (s->pos < first_station->pos){ // The station is in first position
            s->next = first_station;
            first_station->prev = s;
            hw->start = s;
        } else{ // Search for the position of the station
            station * pre = n->prev;
            while(n != NULL && n->pos < s->pos){
                pre = n;
                n = n->next;
            }
            if(n == NULL){ // The station is in last position
                hw->end = s;
                pre->next = s;
                s->prev = pre;
            } else { // The station is in middle position
                pre->next = s;
                s->next = n;
                s->prev = pre;
                n->prev = s;
            }
        }
    }
}

// A function to add a new station
node * addStation(highway * hw, char * command, node * root){
    station * new_station = newStation();
    // Reading distance and number of vehicles in the station
    char * ns;
    new_station->pos = (int) strtol(command, &ns, 10);
    int num_vehicles = (int) strtol(ns + 1, &ns, 10);

    station * neighbour = findStation(root, new_station->pos); // Search for the given position
    if(neighbour != NULL && neighbour->pos == new_station->pos){ // There is another station at that position
        puts("non aggiunta");
        free(new_station);
        return root;
    }

    insertStation(hw, new_station, neighbour); // Insert the station in the linked list
    root = insertNode(root, new_station); // Insert the station in the AVL tree
    puts("aggiunta");

    // Adding vehicles
    for(int i = 0; i < num_vehicles; i++){
        int autonomy = (int) strtol(ns + 1, &ns, 10);
        insertVehicle(new_station, autonomy);
    }
    return root;
}

// A function to find the minimum value node
node * minValueNode(node * n) {
    node * current = n;
    while (current && current->left != NULL)
        current = current->left;

    return current;
}

// A function to delete a node from the AVL tree
node * deleteNode(node * root, station * key) {
    if (root == NULL)
        return root;

    if (key->pos < root->key->pos) // If the station searched is smaller than the current node
        root->left = deleteNode(root->left, key);
    else if(key->pos > root->key->pos) // If the station searched is greater than the current node
        root->right = deleteNode(root->right, key);
    else{  // If the station searched is the current node
        if(root->left == NULL || root->right == NULL){ // If the current node has one or zero children
            node * temp = root->left ? root->left : root->right;
            if (temp == NULL){ // If the current node has zero children
                temp = root;
                root = NULL;
            }
            else // If the current node has one child
                *root = *temp;
            free(temp);
        }
        else{ // If the current node has two children
            node * temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }
    }

    if (root == NULL)
        return root;

    root->height = 1 + max(height(root->left),height(root->right));
    // Check for unbalanced tree
    int balance = getBalance(root);

    // Left - Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0){
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right - Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0){
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}


// A function to remove a station from the linked list
node * removeStation(highway * hw, char * command, node * root){
    char * ns;
    int dist = (int) strtol(command, &ns, 10);

    station * s = findStation(root, dist); // Search for the station
    if (s == NULL || s->pos != dist){ // If the station is not present
        puts("non demolita");
        return root;
    }

    puts("demolita");
    if (s->prev == NULL){ // If the station is in first position
        hw->start = s->next;
        if(s->next != NULL)
            s->next->prev = s->prev;
    } else if(s->next == NULL){ // If the station is in last position
        hw->end = s->prev;
        if(s->prev != NULL)
            s->prev->next = s->next;
    } else{ // If the station is in midlle position
        if(s->prev != NULL)
            s->prev->next = s->next;
        if(s->next != NULL)
            s->next->prev = s->prev;
    }
    root = deleteNode(root, s); // Remove station from the AVL tree
    free(s);
    return root;
}

// A function to compute vehicles' maximum autonomy in a station
void recalculateMaxAutonomy(station * s){
    vehicle * v = s->vehicles;
    s->max_autonomy = 0;
    while(v != NULL){
        if(v->autonomy > s->max_autonomy){
            s->max_autonomy = v->autonomy;
        }
        v = v->next;
    }
}

// A function to remove a vehicle from a station
void removeVehicle(node * root, char * command){
    char * ns;
    int dist = (int) strtol(command, &ns, 10);
    int autonomy = (int) strtol(ns + 1, &ns, 10);
    int flag = 1;
    int search = 0;

    station * s = findStation(root, dist); // Search for the station
    if(s == NULL || s->pos != dist){ // If the station is not present
        puts("non rottamata");
        return;
    }

    // Searching for the vehicle
    vehicle * v = s->vehicles;
    vehicle * pre = NULL;
    while(v != NULL){
        if(v->autonomy == autonomy){
            flag = 0; // The autonomy is present
            if(pre == NULL){ // The vehicle to remove is in first position
                s->vehicles = v->next;
                search = 1;
                free(v);
            } else { // The vehicle to remove is not in first position
                pre->next = v->next;
                search = 1;
                free(v);
            }
            break;
        }
        pre = v;
        v = v->next;
    }

    if (flag == 1)
        puts("non rottamata");
    else
        puts("rottamata");

    // Recalculating maximum autonomy
    if(search == 1 && autonomy == s->max_autonomy)
        recalculateMaxAutonomy(s);
}

// A function to print the found path
void printPath(int * station_names, int * order, int idx, int num_stations){
    if (idx > 0)
        printPath(station_names, order, order[idx], num_stations);
    printf("%d", station_names[idx]);
    if(idx == num_stations - 1){
        fputs("\n", stdout);
    } else{
        fputs(" ", stdout);
    }
}

void calculateMinimumPathForward(int num_stations, station * first, station * last){
    int station_names[num_stations]; // Array containing the stations' positions
    int prev_station[num_stations]; // Array containing the stations' predecessor in the path
    int distances[num_stations]; // Array containing the distances of each station from the first
    station * minimum_station[num_stations]; // Minimum station at that distance
    int minimum_station_distance[num_stations]; // Minimum station's number at that distance
    for(int i = 1; i < num_stations; i++){ // Setting the predecessor to -1
        prev_station[i] = -1;
        distances[i] = -1;
        minimum_station_distance[i] = -1;
        minimum_station[i] = NULL;
    }
    prev_station[0] = 0;
    distances[0] = 0;
    minimum_station_distance[0] = 0;
    minimum_station[0] = first;

    station * position = first; // Starting from first position
    int max_dist = 0; // Farthest distance achieved
    // Calculate the predecessor station for each station
    for(int i = 0; i < num_stations - 1; i++){
        station_names[i] = position->pos;
        if(prev_station[i] == -1) // If the station is not reachable
            break;

        if(position->pos + position->max_autonomy <= max_dist){
            // If the current station cannot achieve a greater distance than the one already achieved
            position = position->next;
            continue;
        }

        station * compare = minimum_station[distances[i]];
        // If the station cannot reach a better distance than a one that has a lower number
        if(position != compare && position->pos + position->max_autonomy < compare->pos + compare->max_autonomy){
            position = position->next;
            continue;
        }

        station * p = position->next;
        int j = 0;
        while (p != NULL && j < num_stations - 1 - i) {
            if (p->pos <= position->pos + position->max_autonomy) { // If the station is reachable
                if (prev_station[j + i + 1] == -1) { // If the station was not reached before
                    prev_station[j + i + 1] = i; // Updates the predecessor
                    distances[j + i + 1] = distances[i] + 1; // Updates distance
                    if (minimum_station_distance[distances[j + i + 1]] == -1 ||
                        minimum_station_distance[distances[j + i + 1]] > j + i + 1){
                        // Updates minimum station's number at that distance
                        minimum_station_distance[distances[j + i + 1]] = j + i + 1;
                        minimum_station[distances[j + i + 1]] = p; // Updates minimum station at that distance
                    }
                    if(position->pos + position->max_autonomy > max_dist){
                        max_dist = position->pos + position->max_autonomy;
                    }
                }
            } else { // If it is not possible to reach other stations
                break;
            }
            j++;
            p = p->next;
        }

        if (prev_station[num_stations - 1] != -1) // If there is already a path to the last station
            break;
        position = position->next;
    }

    station_names[num_stations - 1] = last->pos;

    if(prev_station[num_stations - 1] == -1){ // If the last station is not reachable
        puts("nessun percorso");
        return;
    }

    // Print the path from first station to last
    printPath(station_names, prev_station, num_stations - 1, num_stations);
}

void calculateMinimumPathBackward(int num_stations, station * first, station * last){
    int station_names[num_stations]; // Array containing the stations' positions
    int prev_station[num_stations]; // Array containing the stations' predecessor in the path
    int distances[num_stations]; // Array containing the distances of each station from the first
    station * minimum_station[num_stations]; // Minimum station at that distance
    int minimum_station_distance[num_stations]; // Minimum station's number at that distance
    for(int i = 1; i < num_stations; i++){ // Setting the predecessor to -1
        prev_station[i] = -1;
        distances[i] = -1;
        minimum_station_distance[i] = -1;
        minimum_station[i] = NULL;
    }
    prev_station[0] = 0;
    distances[0] = 0;
    minimum_station_distance[0] = 0;
    minimum_station[0] = first;

    station * position = first; // Starting from first position
    // Calculate the predecessor station for each station
    for(int i = 0; i < num_stations - 1; i++){
        station_names[i] = position->pos;
        if(prev_station[i] == -1) // If the station is not reachable
            break;

        station * compare = minimum_station[distances[i]];
        // If the station cannot reach a better distance than a one that has a lower number
        if(position != compare && position->pos - position->max_autonomy > compare->pos - compare->max_autonomy){
            position = position->prev;
            continue;
        }

        station * p = position -> prev;
        int j = 0;
        while(p != NULL && j < num_stations - i - 1){
            if(p->pos >= position->pos - position->max_autonomy){
                if(prev_station[j + i + 1] == -1 ||
                distances[j + i + 1] >= distances[i] + 1){ // If the station was not reached before
                    prev_station[j + i + 1] = i; // Updates predecessor
                    distances[j + i + 1] = distances[i] + 1; // Updates distance
                    if (minimum_station_distance[distances[j + i + 1]] == -1 ||
                            minimum_station_distance[distances[j + i + 1]] < j + i + 1){
                        // Updates minimum station's number at that distance
                        minimum_station_distance[distances[j + i + 1]] = j + i + 1;
                        minimum_station[distances[j + i + 1]] = p; // Updates minimum station at that distance
                    }
                }
            } else{
                break;
            }
            j ++;
            p = p->prev;
        }
        position = position->prev;
    }

    station_names[num_stations - 1] = last->pos;

    if(prev_station[num_stations - 1] == -1){ // If the last station is not reachable
        fputs("nessun percorso\n", stdout);
        return;
    }

    // Print the path from first station to last
    printPath(station_names, prev_station, num_stations - 1, num_stations);
}

void calculatePath(node * root, char * command){
    // Reading the two station
    char *ns;
    int sta1 = (int) strtol(command, &ns, 10);
    int sta2 = (int) strtol(ns + 1, &ns, 10);

    if(sta1 == sta2){ // If the two station are the same, return only the station
        printf("%d\n", sta1);
    } else if(sta1 < sta2){
        station * first_station = findStation(root, sta1); // Find first station
        station * p = first_station;
        int count = 1;

        // Count number of stations between the first and the second
        while(p->pos != sta2){
            count ++;
            p = p->next;
        }
        station * second_station = p;
        calculateMinimumPathForward(count, first_station, second_station); // Calculate pathway
    } else{
        station * first_station = findStation(root, sta1); // Find first station
        station * p = first_station;
        int count = 1;

        // Count number of stations between the first and the second
        while(p->pos != sta2){
            count ++;
            p = p->prev;
        }
        station * second_station = p;
        calculateMinimumPathBackward(count, first_station, second_station); // Calculate pathway
    }
}

int main() {
    char command[10000];
    highway * hw = malloc(sizeof(highway));
    hw->start = NULL;
    hw->end = NULL;
    node * root = NULL;

    while(fgets(command, 10000, stdin) != NULL){ // While loops that reads the commands
        switch (command[0]){
            case 'a':
                switch (command[9]){
                    case 's': // Add a station
                        root = addStation(hw, &command[17], root);
                        break;
                    case 'a': // Add a vehicle
                        addVehicle(root, &command[13]);
                        break;
                }
                break;
            case 'd': // Remove a station
                root = removeStation(hw, &command[18], root);
                break;
            case 'r': // Remove a vehicle
                removeVehicle(root, &command[12]);
                break;
            case 'p': // Find pathway
                calculatePath(root, &command[18]);
                break;
            default:
                puts("Error: wrong command");
        }
    }
    return 0;
}
