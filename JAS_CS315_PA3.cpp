
// JAS_CS315_PA3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//Written by Jonathan Stilz
//ChatGPT Prompt: "in c, give a way to read a text file that holds data seperated by commas"

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <algorithm>
#include <string>
#include <iostream>
//#include <cstring>
#include <queue>


#define MAX_LINE_LENGTH 1000
#define MAX_FIELDS 20
#define MAX_POINTS 100
#define MAX_DISTANCE 1000000



int my_strcmp(const char* str1, const char* str2) {
    while (*str1 && *str2) {
        if (*str1 < *str2) {
            return -1; // str1 is less than str2

        }
        else if (*str1 > *str2) {
            return 1; // str1 is greater than str2
        }
        str1++;
        str2++;
    }

    // If both strings are equal so far, but one may be shorter
    if (*str1 == '\0' && *str2 == '\0') {
        return 0; // Both strings are equal
    }
    else if (*str1 == '\0') {
        return -1; // str1 is shorter
    }
    else {
        return 1; // str2 is shorter
    }
}

class Edge;

class Point {
public:
    Point* predecessor = nullptr;
    char* name;
    Edge* nextEdge = nullptr;
    int color = 0; //0 = white , 1 = gray , 2 = black
    int distance = MAX_DISTANCE;
    Point(char* name) : name(name) {}
};

class Edge {
public:
    int cost;
    Point* point;
    Edge* nextEdge = nullptr;

    // Constructor
    Edge(Point* point, int cost) : point(point), cost(cost) {}
};

void BFS(Point* pointArray[], int pointArrayLength, char* sourceStr, char* destStr) {
    //reset distances
    for (int i = 0; i < pointArrayLength; i++) {
        pointArray[i]->distance = MAX_DISTANCE;
        pointArray[i]->predecessor = nullptr;
        pointArray[i]->color = 0;
    }
    //delcare queue for points
    std::queue<Point*> pointQueue;
    //find source point
    Point* sourcePoint = nullptr;
    for (int i = 0; i < pointArrayLength; i++) {
        if (!my_strcmp(pointArray[i]->name, sourceStr)) {
            sourcePoint = pointArray[i];
            break;
        }
    }

    //find destination point
    Point* destPoint = nullptr;
    for (int i = 0; i < pointArrayLength; i++) {
        if (!my_strcmp(pointArray[i]->name, destStr)) {
            destPoint = pointArray[i];
            break;
        }
    }
    //check for error
    if (destPoint == nullptr || sourcePoint == nullptr) {
        std::cout << "Error!" << std::endl;
        return;
    }

    //declare current point
    Point* currPoint = sourcePoint;
    //queue source
    pointQueue.push(sourcePoint);
    //make currPoint 0 distance
    currPoint->distance = 0;
    while (true) {
        //remove currPoint from queue
        pointQueue.pop();
        int distance = currPoint->distance + 1;
        //turn point black
        currPoint->color = 2;
        //queue edge points
        Edge* currEdge = currPoint->nextEdge;
        while (currEdge != nullptr) {
            //add point if white
            if (currEdge->point->color == 0) {
                currEdge->point->color = 1; //turn gray
                pointQueue.push(currEdge->point);
                //std::cout << "queued " << currEdge->point->name << "\n";
                //give predecessor
                currEdge->point->predecessor = currPoint;
                //give distance
                currEdge->point->distance = currPoint->distance + 1;
            }
            currEdge = currEdge->nextEdge;
        }

        //check to break loop
        if (!pointQueue.empty()) {
            //std::cout << "dequeued " << pointQueue.front()->name << " with distance " << pointQueue.front()->distance << "\n";
            currPoint = pointQueue.front();
        }
        else { break; }

    }
}



int Parent(int child) {
    return (child - 2) / 2;
}

int Left(int parent) {
    return (parent * 2 + 1);
}
int Right(int parent) {
    return (parent * 2 + 2);
}

void MaxHeapify(Point* (&pointArray)[MAX_POINTS], int index) {
    int left = Left(index);
    int right = Right(index);

    //if left is less than root
    if (pointArray[left] != nullptr && pointArray[left]->distance < pointArray[index]->distance) {
        //swap left with root
        Point* swap = pointArray[left];
        pointArray[left] = pointArray[index];
        pointArray[index] = swap;
        //check child after swap

    }

    //if right is greater than root
    if (pointArray[right] != nullptr && pointArray[right]->distance < pointArray[index]->distance) {
        //swap right with root
        Point* swap = pointArray[right];
        pointArray[right] = pointArray[index];
        pointArray[index] = swap;
        //check child after swap

    }
    if (pointArray[left] != nullptr && pointArray[right] != nullptr) {
        //check if left is greatest
        if (pointArray[left]->distance > pointArray[right]->distance && pointArray[left]->distance > pointArray[index]->distance) {
            MaxHeapify(pointArray, left);
        }
        //check if right is greatest
        else if (pointArray[left]->distance < pointArray[right]->distance && pointArray[right]->distance > pointArray[index]->distance) {
            MaxHeapify(pointArray, right);
        }
    }
}

void BuildMaxHeap(Point* (&pointArray)[MAX_POINTS], int pointArrayLength) {
    int startIndex = (pointArrayLength - 2) / 2;
    for (int i = startIndex; i >= 0; i--) {
        MaxHeapify(pointArray, i);
    }
}

Point* HeapDequeueMin(Point* (&pointArray)[MAX_POINTS], int& pointArrayLength) {
    if (!pointArrayLength) {
        std::cout << "Queue empty when tried to dequeue\n";
        return nullptr;
    }
    Point* dequeueVal = pointArray[0];
    if (pointArrayLength == 1) {
        pointArray[0] = nullptr;
        pointArrayLength--;
    }
    else {
        pointArray[0] = pointArray[pointArrayLength - 1];
        pointArray[pointArrayLength - 1] = nullptr;
        pointArrayLength--;
        MaxHeapify(pointArray, 0);
    }

    return dequeueVal;
}
void HeapDecreaseKey(Point* (&pointArray)[MAX_POINTS], int index, int distance) {
    if (distance > pointArray[index]->distance) {

        std::cout << "Error? new key is greater than current key\n";

    }
    else {
        pointArray[index]->distance = distance;
        while (index > 0 && pointArray[Parent(index)]->distance > pointArray[index]->distance) {
            //swap child with parent
            Point* swap = pointArray[Parent(index)];
            pointArray[Parent(index)] = pointArray[index];
            pointArray[index] = swap;
            //change index to parent
            index = Parent(index);
        }
    }
    //MaxHeapify(pointArray, );
}

void HeapAddPoint(Point* (&pointQueue)[MAX_POINTS], Point* point, int& pointArrayLength) {
    //increment length
    pointArrayLength++;
    //insert point
    pointQueue[pointArrayLength - 1] = point;
    HeapDecreaseKey(pointQueue, pointArrayLength - 1, point->distance);
}

int findIndex(Point* pointQueue[], Point* point, int pointArrayLength) {
    for (int i = 0; i < pointArrayLength; i++) {
        if (pointQueue[i] == point) {
            return i;
        }
    }
    return -1;
}

void Dijkstra(Point* (&pointArray)[MAX_POINTS], int pointArrayLength, char* sourceStr, char* destStr) {
    //reset distances
    for (int i = 0; i < pointArrayLength; i++) {
        pointArray[i]->distance = MAX_DISTANCE;
        pointArray[i]->predecessor = nullptr;
        pointArray[i]->color = 0;
    }
    //delcare queue for points
    Point* pointQueue[MAX_POINTS];
    for (int i = 0; i < MAX_POINTS; i++) {
        pointQueue[i] = nullptr;
    }
    int pointQueueLength = 1;
    //find source point
    Point* sourcePoint = nullptr;
    for (int i = 0; i < pointArrayLength; i++) {
        if (!my_strcmp(pointArray[i]->name, sourceStr)) {
            sourcePoint = pointArray[i];
            break;
        }
    }

    //find destination point
    Point* destPoint = nullptr;
    for (int i = 0; i < pointArrayLength; i++) {
        if (!my_strcmp(pointArray[i]->name, destStr)) {
            destPoint = pointArray[i];
            break;
        }
    }
    //check for error
    if (destPoint == nullptr || sourcePoint == nullptr) {
        std::cout << "Error!" << std::endl;
        return;
    }

    //declare current point
    Point* currPoint = sourcePoint;
    //queue source
    pointQueue[0] = sourcePoint;
    //make currPoint 0 distance
    currPoint->distance = 0;
    while (true) {
        //remove currPoint from queue
        currPoint = HeapDequeueMin(pointQueue, pointQueueLength);
        int distance = currPoint->distance + 1;
        //turn point black
        currPoint->color = 2;
        //queue edge points
        Edge* currEdge = currPoint->nextEdge;
        while (currEdge != nullptr) {
            //calculate the cost of the path for the else-if statement
            //int pathDistance = (currEdge->cost + currPoint->predecessor->distance);
            //add point if white
            if (currEdge->point->color == 0) {
                currEdge->point->color = 1; //turn gray
                //std::cout << "queued " << currEdge->point->name << "\n";
                //give predecessor

                currEdge->point->predecessor = currPoint;
                //give distance
                currEdge->point->distance = currPoint->distance + currEdge->cost;
                //nonsesnse test
                HeapAddPoint(pointQueue, currEdge->point, pointQueueLength);
                //HeapAddPoint(pointQueue, currPoint, pointQueueLength);
            }
            //check if gray edges need an updated value
            /*
            else if (currEdge->point->color == 1 && currPoint->predecessor == nullptr) {
                currEdge->point->predecessor = currPoint;
                currEdge->point->distance = (currEdge->cost + currPoint->distance);
            }
            */
            else if (currEdge->point->color == 1 && (currEdge->cost + currPoint->distance) < currEdge->point->distance) {
                currEdge->point->predecessor = currPoint;

                HeapDecreaseKey(pointQueue, findIndex(pointQueue, currEdge->point, pointQueueLength), currEdge->cost + currPoint->distance);
                //currEdge->point->distance = (currEdge->cost + currPoint->distance);
            }
            currEdge = currEdge->nextEdge;
        }

        //check to break loop
        if (pointQueueLength != 0) {
            //std::cout << "dequeued " << pointQueue[0]->name << " with distance " << pointQueue[0]->distance << "\n";
            currPoint = pointQueue[0];
        }
        else { break; }

    }

}

void PrintPath(Point* pointArray[], Point* start, Point* dest, int curr_distance = 0) {
    if (start->name == dest->name) {
        std::cout << start->name << " ---> ";
    }
    else if (dest->predecessor == nullptr) {
        std::cout << "Could not find a path!\n";
    }
    else {
        PrintPath(pointArray, start, dest->predecessor, curr_distance + 1);
        std::cout << dest->name;
        if (curr_distance) {
            std::cout << " ---> ";
        }
        else {
            std::cout << std::endl;
        }
    }
}
void Prim(Point* pointArray[], Point* start, int pointArrayLength) {
    //reset distances
    Point* pointQueue[MAX_POINTS];
    for (int i = 0; i < pointArrayLength; i++) {
        pointArray[i]->distance = MAX_DISTANCE;
        pointArray[i]->predecessor = nullptr;
        pointQueue[i] = pointArray[i];
    }
    start->distance = 0;
    
    int pointQueueLength = pointArrayLength;
    pointQueue[0] = start;
    
    while (pointQueueLength != 0) {
        Point* currPoint = HeapDequeueMin(pointQueue, pointQueueLength);
        Edge* currEdge = currPoint->nextEdge;
        
        while (currEdge != nullptr) {
            bool inList = false;
            int index = 0;
            for (int i = 0; i < pointQueueLength; i++) {
                
                if (currEdge->point == pointQueue[i]) {
                    inList = true;
                    index = i;
                    break;
                }
                
            }
            if (inList && currEdge->cost < currEdge->point->distance) {
                currEdge->point->predecessor = currPoint;
                HeapDecreaseKey(pointQueue, index, currEdge->cost);
            }
            currEdge = currEdge->nextEdge;
        }
        
    }
    for (int i = 0; i < pointArrayLength ; i++) {
        if (pointArray[i]->predecessor) {

            std::cout << "The predecessor of " << pointArray[i]->name << " is " << pointArray[i]->predecessor->name << std::endl;
        }
        else {
            std::cout << pointArray[i]->name << " has no predecessor " << std::endl;
        }
    }


}

int main() {

    FILE* file;
    errno_t err;

    //declare map to hold hash table
    Point* pointArray[MAX_POINTS];
    int pointArrayLength = 0;
    //std::map<std::string, Node*> midEarthMap;



    err = fopen_s(&file, "MiddleEarthVertices.txt", "r");
    if (err != 0) {
        //fprintf(stderr, "Error opening the file.\n");
        return 1;
    }

    // Buffer to hold each line
    char line[MAX_LINE_LENGTH];

    // Read each line from the file
    int index = 0;
    //index for pointArray
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        line[strcspn(line, "\n")] = '\0';
        // Tokenize the line based on commas
        char* token;
        char* saveptr; // for strtok_r()

        // Tokenize the line based on commas
        token = strtok_s(line, ",", &saveptr);

        // Process each field in the line

        //while (token != NULL) {
        //printf("%s\n", token);
        //add all points to pointArray
        //increment size

        //token = strtok_s(NULL, ",", &saveptr);
        char* dupstring = _strdup(token);
        pointArray[index] = new Point(dupstring);
        pointArrayLength++;


        //}
        index++;
    }

    // Close the file
    fclose(file);
    //debug statement
    err = fopen_s(&file, "MiddleEarthEdges.txt", "r");
    if (err != 0) {
        fprintf(stderr, "Error opening the file.\n");
        return 1;
    }



    // Read each line from the file
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        line[strcspn(line, "\n")] = '\0';
        // Tokenize the line based on commas
        char* token;
        char* saveptr; // for strtok_r()

        // Tokenize the line based on commas
        token = strtok_s(line, ",", &saveptr);

        // Process each field in the line
        int index = 0;
        //give nonsense initialization points
        Point* starting_point = pointArray[0];
        Point* destination_point = pointArray[0];
        while (token != NULL) {
            //printf("%s\n", token);

            //find starting point
            if (index == 0) {

                for (int i = 0; i < pointArrayLength; i++) {
                    int result = my_strcmp(token, pointArray[i]->name);
                    if (result == 0) {
                        //printf("I ran!\n");
                        starting_point = pointArray[i];
                        break;
                    }
                }
                //printf("error!\n");
            }
            //find find destination point

            else if (index == 1) {
                for (int i = 0; i < pointArrayLength; i++) {
                    if (my_strcmp(token, pointArray[i]->name) == 0) {
                        //printf("I ran2!\n");
                        destination_point = pointArray[i];
                        break;
                    }
                }
                //printf("error2!\n");

            }

            //create edge

            else {
                int cost = std::atoi(token);
                //go to end of list of edges

                //if first edge
                if (starting_point->nextEdge == nullptr) {
                    starting_point->nextEdge = new Edge(destination_point, cost);
                }
                //if not first edge
                else {
                    Edge* currEdge = starting_point->nextEdge;
                    //go to last edge
                    while (currEdge->nextEdge != nullptr) {
                        currEdge = currEdge->nextEdge;
                    }
                    //add new edge
                    currEdge->nextEdge = new Edge(destination_point, cost);
                }
            }

            //increment token
            token = strtok_s(NULL, ",", &saveptr);
            index++;
        }
    }

    // Close the file
    fclose(file);

    int index2 = 0;
    Edge* currEdge = nullptr;
    Point* currPoint = pointArray[0];
    while (index2 < pointArrayLength) {
        currPoint = pointArray[index2];
        std::cout << currPoint->name << ": ";

        currEdge = currPoint->nextEdge;
        while (currEdge) {
            std::cout << " " << currEdge->point->name << "(" << currEdge->cost << ")," << "\t";
            currEdge = currEdge->nextEdge;
        }
        index2++;
        std::cout << "\n";
    }
    char sourceStr[] = "Hobbiton";
    char destStr[] = "MountDoom";
    Point* sourcePoint = nullptr;
    Point* destPoint = nullptr;
    //make copy of array for Dijkrstra's
    BFS(pointArray, pointArrayLength, sourceStr, destStr);
    //find index for print path
    for (int i = 0; i < pointArrayLength; i++) {
        if (my_strcmp(pointArray[i]->name, sourceStr) == 0) {
            sourcePoint = pointArray[i];
        }
        else if (my_strcmp(pointArray[i]->name, destStr) == 0) {
            destPoint = pointArray[i];
        }
    }
    std::cout << std::endl;
    std::cout << "Shortest Path from Hobbiton to Mt. Doom using BFS:\n";
    PrintPath(pointArray, sourcePoint, destPoint);
    std::cout << std::endl;
    
    std::cout << "Shortest Path from Hobbiton to Mt.Doom using Dijkstra's:\n";
    Dijkstra(pointArray, pointArrayLength, sourceStr, destStr);
    PrintPath(pointArray, sourcePoint, destPoint);
    std::cout << std::endl;
    //std::cout << "The predecessor structure of Prim's Algorithm(starting at Hobbiton) is:\n";
    //Prim(pointArray, sourcePoint, pointArrayLength);
    return 0;
}
