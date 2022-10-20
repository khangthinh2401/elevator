#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <math.h>
#include <queue>
#include <windows.h>

#define ll long long
#define vdb vector<double>
#define vll vector<long long>
using namespace std;

struct GUEST
{
    int current_floor;
    int dest_floor;
    string direction;
    GUEST *next;
};
struct ELEVATOR
{
    int current_floor;
    string state;
    int d1, d2, d3;
};

GUEST *createGuest(int _current_floor, int _dest_floor, string _direction);
GUEST *deleteGuest(GUEST *list, int position, int &size);
GUEST *insertGuest(GUEST *list, int current_floor, int dest_floor, string direction, int &size);
void solveUp(GUEST *uplist, GUEST *downlist, ELEVATOR &elevator);
void solveDown(GUEST *uplist, GUEST *downlist, ELEVATOR &elevator);
queue<GUEST> getuplist(GUEST *uplist, ELEVATOR &elevator);
queue<GUEST> getdownlist(GUEST *downlist, ELEVATOR &elevator);
void printList(GUEST *list);
bool compareFloorIncrease(GUEST &g1, GUEST &g2);
bool compareFloorDecrease(GUEST &g1, GUEST &g2);
int getLowestCurrentFloor(GUEST *list);
int getHighestCurrentFloor(GUEST *list);

vector<GUEST> vt_dest;
int uplistSize = 0;
int queueUpSize = 0;
int downlistSize = 0;
int queueDownSize = 0;

int main()
{
    ELEVATOR elevator = {2, "NEUTRAL", 0, 0, 0};
    queue<GUEST> q;
    GUEST *uplist = nullptr;
    GUEST *downlist = nullptr;
    int n;
    cout << "CURRENT ELEVATOR FLOOR: " << elevator.current_floor << endl;
    cout << "--------------------------\n";
    cout << "Number of users: ";
    cin >> n;
    cout << "--------------------------\n";
    int current_floor, dest_floor;
    string direction;
    for (int i = 0; i < n; i++)
    {
        cout << "User no." << i + 1 << endl;
        cout << "up/down:       ";
        cin >> direction;
        cout << "Current floor: ";
        cin >> current_floor;
        cout << "Dest. floor:   ";
        cin >> dest_floor;
        GUEST temp = {current_floor, dest_floor, direction, nullptr};
        q.push(temp);
        if (direction == "up")
        {
            uplist = insertGuest(uplist, current_floor, dest_floor, direction, uplistSize);
            uplistSize++;
            queueUpSize++;
        }
        else
        {
            downlist = insertGuest(downlist, current_floor, dest_floor, direction, downlistSize);
            downlistSize++;
            queueDownSize++;
        }
        cout << endl;
    }
    if (q.front().current_floor == elevator.current_floor)
    {
        if (q.front().direction == "up")
        {
            solveUp(uplist, downlist, elevator);
        }
        else
        {
            solveDown(uplist, downlist, elevator);
        }
    }
    else if (q.front().current_floor < elevator.current_floor)
    {
        solveDown(uplist, downlist, elevator);
    }
    else
    {
        solveUp(uplist, downlist, elevator);
    }
    return 0;
}

void printList(GUEST *list)
{
    GUEST *p = list;
    while (p != nullptr)
    {
        cout << p->current_floor << " ";
        p = p->next;
    }
}
GUEST *createGuest(int current_floor, int dest_floor, string direction)
{
    GUEST *temp = new GUEST{current_floor, dest_floor, direction, nullptr};
    return temp;
}
GUEST *deleteGuest(GUEST *list, int position, int &size)
{
    GUEST *p = list;
    if (position == 0)
    {
        p = p->next;
        delete (list);
        size--;
        return p;
    }
    else if (position == size - 1)
    {
        while (p->next->next != nullptr)
        {
            p = p->next;
        }
        delete (p->next);
        size--;
        p->next = nullptr;
        return list;
    }
    else
    {
        for (int i = 0; i < position - 1; i++)
        {
            p = p->next;
        }
        GUEST *temp = p->next;
        p->next = p->next->next;
        delete (temp);
        size--;
        return list;
    }
    return list;
}
GUEST *insertGuest(GUEST *list, int current_floor, int dest_floor, string direction, int &size)
{
    if (size == 0)
    {
        list = createGuest(current_floor, dest_floor, direction);
        return list;
    }
    GUEST *p = list;
    while (p->next != nullptr)
    {
        p = p->next;
    }
    GUEST *temp = new GUEST{current_floor, dest_floor, direction, nullptr};
    p->next = temp;
    return list;
}
void solveUp(GUEST *uplist, GUEST *downlist, ELEVATOR &elevator)
{
    queue<GUEST> q = getuplist(uplist, elevator);
    int highestFloor = getHighestCurrentFloor(downlist);
    elevator.state = "GOINGUP";
    cout << "\n----------THE ELEVATOR IS GOING UP----------" << endl;
    while (elevator.current_floor <= 12 && !vt_dest.empty() || (elevator.current_floor <= 12 && elevator.current_floor <= highestFloor))
    {
        bool check = false;
        cout << "The elevator is at floor: " << elevator.current_floor;
        Sleep(500);
        if (elevator.current_floor != 12)
        {
            printf("\33[2K\r");
        }
        // get user in, have user out
        if (!q.empty() && !vt_dest.empty() && (elevator.current_floor == q.front().current_floor) && (elevator.current_floor == vt_dest[0].dest_floor))
        {
            if (elevator.current_floor == 12)
            {
                printf("\33[2K\r");
            }
            cout << "The elevator is at floor: " << elevator.current_floor << " - USER IN, USER OUT\n";
            // delete all people have been waiting at this floor
            while (!q.empty() && q.front().current_floor <= elevator.current_floor)
            {
                if (q.front().current_floor < elevator.current_floor)
                {
                    uplist = insertGuest(uplist, q.front().current_floor, q.front().dest_floor, q.front().direction, uplistSize);
                    uplistSize++;
                    check = true;
                }
                q.pop();
                queueUpSize--;
                if (check)
                {
                    queueUpSize++;
                }
            }
            // delete all people go out at this floor
            while (!vt_dest.empty() && vt_dest[0].dest_floor == elevator.current_floor)
            {
                vt_dest.erase(vt_dest.begin());
            }
            cout << "\t\t----\n";
            elevator.current_floor++;
            continue;
        }
        // only user in
        if (!q.empty() && elevator.current_floor == q.front().current_floor)
        {
            if (elevator.current_floor == 12)
            {
                printf("\33[2K\r");
            }
            cout << "The elevator is at floor: " << elevator.current_floor << " - USER IN\n";
            // delete all people have been waiting at this floor
            while (!q.empty() && q.front().current_floor <= elevator.current_floor)
            {
                if (q.front().current_floor < elevator.current_floor)
                {
                    uplist = insertGuest(uplist, q.front().current_floor, q.front().dest_floor, q.front().direction, uplistSize);
                    uplistSize++;
                    check = true;
                }
                q.pop();
                queueUpSize--;
                if (check)
                {
                    queueUpSize++;
                }
            }
            cout << "\t\t----\n";
            elevator.current_floor++;
            continue;
        }
        // user out
        if (!vt_dest.empty() && elevator.current_floor == vt_dest[0].dest_floor)
        {
            if (elevator.current_floor == 12)
            {
                printf("\33[2K\r");
            }
            cout << "The elevator is at floor: " << elevator.current_floor << " - USER OUT\n";
            // delete all people go out at this floor
            while (!vt_dest.empty() && vt_dest[0].dest_floor == elevator.current_floor)
            {
                vt_dest.erase(vt_dest.begin());
            }
            cout << "\t\t----\n";
            elevator.current_floor++;
            continue;
        }
        elevator.current_floor++;
    }
    elevator.current_floor--;
    if (queueUpSize > 0 || queueDownSize > 0)
    {
        solveDown(uplist, downlist, elevator);
    }
}
void solveDown(GUEST *uplist, GUEST *downlist, ELEVATOR &elevator)
{

    queue<GUEST> q = getdownlist(downlist, elevator);
    int lowestFloor = getLowestCurrentFloor(uplist);
    elevator.state = "GOINGDOWN";
    cout << "\n---------THE ELEVATOR IS GOING DOWN---------" << endl;
    while (elevator.current_floor >= 1 && (!vt_dest.empty() || elevator.current_floor >= lowestFloor))
    {
        bool check = false;
        cout << "The elevator is at floor: " << elevator.current_floor;
        Sleep(500);
        if (elevator.current_floor != 1)
        {
            printf("\33[2K\r");
        }
        // get user in, have user out
        if (!q.empty() && !vt_dest.empty() && (elevator.current_floor == q.front().current_floor) && (elevator.current_floor == vt_dest[0].dest_floor))
        {
            if (elevator.current_floor == 1)
            {
                printf("\33[2K\r");
            }
            cout << "The elevator is at floor: " << elevator.current_floor << " - USER IN, USER OUT\n";
            // delete all people have been waiting at this floor
            while (!q.empty() && q.front().current_floor >= elevator.current_floor)
            {
                if (q.front().current_floor > elevator.current_floor)
                {
                    downlist = insertGuest(downlist, q.front().current_floor, q.front().dest_floor, q.front().direction, downlistSize);
                    downlistSize++;
                    check = true;
                }
                q.pop();
                queueDownSize--;
                if (check)
                {
                    queueDownSize++;
                }
            }
            // delete all people go out at this floor
            while (!vt_dest.empty() && vt_dest[0].dest_floor == elevator.current_floor)
            {
                vt_dest.erase(vt_dest.begin());
            }
            cout << "\t\t----\n";
            elevator.current_floor--;
            continue;
        }
        // only user in
        if (!q.empty() && elevator.current_floor == q.front().current_floor)
        {
            if (elevator.current_floor == 1)
            {
                printf("\33[2K\r");
            }
            cout << "The elevator is at floor: " << elevator.current_floor << " - USER IN\n";
            // delete all people have been waiting at this floor
            while (!q.empty() && q.front().current_floor >= elevator.current_floor)
            {
                if (q.front().current_floor > elevator.current_floor)
                {
                    downlist = insertGuest(downlist, q.front().current_floor, q.front().dest_floor, q.front().direction, downlistSize);
                    downlistSize++;
                    check = true;
                }
                q.pop();
                queueDownSize--;
                if (check)
                {
                    queueDownSize++;
                }
            }
            cout << "\t\t----\n";
            elevator.current_floor--;
            continue;
        }
        // user out
        if (!vt_dest.empty() && elevator.current_floor == vt_dest[0].dest_floor)
        {
            if (elevator.current_floor == 1)
            {
                printf("\33[2K\r");
            }
            cout << "The elevator is at floor: " << elevator.current_floor << " - USER OUT\n";
            // delete all people go out
            while (!vt_dest.empty() && vt_dest[0].dest_floor == elevator.current_floor)
            {
                vt_dest.erase(vt_dest.begin());
            }
            cout << "\t\t----\n";
            elevator.current_floor--;
            continue;
        }
        elevator.current_floor--;
    }
    elevator.current_floor++;
    if (queueUpSize > 0 || queueDownSize > 0)
    {
        solveUp(uplist, downlist, elevator);
    }
}
queue<GUEST> getuplist(GUEST *uplist, ELEVATOR &elevator)
{
    vt_dest.clear();
    queue<GUEST> result;
    GUEST *p = uplist;
    GUEST *firstGuest = uplist;
    int i = 0;
    while (p != nullptr)
    {
        if (p->current_floor >= elevator.current_floor)
        {
            result.push(*p);
            vt_dest.push_back(*p);
            p = p->next;
            uplist = deleteGuest(uplist, i, uplistSize);
            continue;
        }
        p = p->next;
        i++;
    }
    // sort vector
    if (!vt_dest.empty())
    {
        sort(vt_dest.begin(), vt_dest.end(), compareFloorIncrease);
    }
    return result;
}
queue<GUEST> getdownlist(GUEST *downlist, ELEVATOR &elevator)
{
    vt_dest.clear();
    queue<GUEST> result;
    GUEST *p = downlist;
    int i = 0;
    while (p != nullptr)
    {
        if (p->current_floor <= elevator.current_floor)
        {
            result.push(*p);
            vt_dest.push_back(*p);
            p = p->next;
            downlist = deleteGuest(downlist, i, downlistSize);
            continue;
        }
        p = p->next;
        i++;
    }
    // sort vector
    if (!vt_dest.empty())
    {
        sort(vt_dest.begin(), vt_dest.end(), compareFloorDecrease);
    }
    return result;
}
int getLowestCurrentFloor(GUEST *list)
{
    if (uplistSize <= 0)
    {
        return 13;
    }
    GUEST *p = list;
    int lowest = p->current_floor;
    p = p->next;
    while (p != nullptr)
    {
        if (p->current_floor < lowest)
        {
            lowest = p->current_floor;
        }
        p = p->next;
    }
    return lowest;
}
int getHighestCurrentFloor(GUEST *list)
{
    if (downlistSize <= 0)
    {
        return -1;
    }
    GUEST *p = list;
    int max = p->current_floor;
    p = p->next;
    while (p != nullptr)
    {
        if (p->current_floor > max)
        {
            max = p->current_floor;
        }
        p = p->next;
    }
    return max;
}
bool compareFloorIncrease(GUEST &g1, GUEST &g2)
{
    return g1.dest_floor < g2.dest_floor;
}
bool compareFloorDecrease(GUEST &g1, GUEST &g2)
{
    return g1.dest_floor > g2.dest_floor;
}