#include <iostream>
#include <algorithm>
#include <string>
#include <windows.h>
#include <queue>

using namespace std;
struct GUEST
{
    int current_floor;
    int floor_to_go;
    string flag;
};
struct ELEVATOR
{
    int CURRENT_FLOOR;
    string STATE;
    int D1;
    int D2;
    int D3;
};
struct FLOOR
{
    int floor_number;
    FLOOR *next;
};
FLOOR *createFloor(int x);
FLOOR *insertTail(FLOOR *list, int x, int &size);
int findPositionByValue(FLOOR *list, int value, int size);
FLOOR *deleteFloor(FLOOR *list, int position, int &size);
int findNextFloor_Up(FLOOR *uplist, int current_floor, int &size);
int findNextFloor_Down(FLOOR *downlist, int current_floor, int &size);
void solveUp(FLOOR *waitUp, FLOOR *waitDown, FLOOR *uplist, FLOOR *downlist, ELEVATOR &elevator);
void solveDown(FLOOR *waitUp, FLOOR *waitDown, FLOOR *uplist, FLOOR *downlist, ELEVATOR &elevator);

int uplistSize = 0;
int waitUpSize = 0;
int downlistSize = 0;
int waitDownSize = 0;
int highestFloor = 1;
int lowestFloor = 12;

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    system("cls");

    ELEVATOR elevator = {2, "NEUTRAL", 0, 0, 0};
    // chứa ds tầng cần đến
    FLOOR *uplist = new FLOOR;
    // chứa ds tầng cần xuống
    FLOOR *downlist = new FLOOR;
    // chứa ds người cần đi lên
    FLOOR *waitUp = new FLOOR;
    // chứa ds người cần đi xuống
    FLOOR *waitDown = new FLOOR;
    // hàng đợi người
    queue<GUEST> q;

    int n;
    cout << "CURRENT ELEVATOR FLOOR: " << elevator.CURRENT_FLOOR << endl;
    cout << "--------------------------\n";
    cout << "Number of users: ";
    cin >> n;
    cout << "--------------------------\n";
    int current_floor_temp, floor_temp;
    string flag_temp;
    for (int i = 0; i < n; i++)
    {
        cout << "User no." << i + 1 << endl;
        cout << "(up/down):     ";
        cin >> flag_temp;
        cout << "Current floor: ";
        cin >> current_floor_temp;
        cout << "Dest. floor:   ";
        cin >> floor_temp;
        GUEST guest = {current_floor_temp, floor_temp, flag_temp};
        if (flag_temp == "up")
        {
            uplist = insertTail(uplist, floor_temp, uplistSize);
            waitUp = insertTail(waitUp, current_floor_temp, waitUpSize);
            uplistSize++;
            waitUpSize++;
            if (current_floor_temp < lowestFloor)
            {
                lowestFloor = current_floor_temp;
            }
        }
        else
        {
            downlist = insertTail(downlist, floor_temp, downlistSize);
            waitDown = insertTail(waitDown, current_floor_temp, waitDownSize);
            downlistSize++;
            waitDownSize++;
            if (current_floor_temp > highestFloor)
            {
                highestFloor = current_floor_temp;
            }
        }
        cout << endl;
        q.push(guest);
    }
    if (q.front().current_floor == elevator.CURRENT_FLOOR)
    {
        printf("\e[?25l");
        if (q.front().flag == "up")
        {
            solveUp(waitUp, waitDown, uplist, downlist, elevator);
        }
        else
        {
            solveDown(waitUp, waitDown, uplist, downlist, elevator);
        }
    }
    else if (q.front().current_floor > elevator.CURRENT_FLOOR)
    {
        solveUp(waitUp, waitDown, uplist, downlist, elevator);
    }
    else
    {
        solveDown(waitUp, waitDown, uplist, downlist, elevator);
    }
    return 0;
}

void solveUp(FLOOR *waitUp, FLOOR *waitDown, FLOOR *uplist, FLOOR *downlist, ELEVATOR &elevator)
{
    elevator.STATE = "GOINGUP";
    cout << "\n----------THE ELEVATOR IS GOING UP----------" << endl;
    while (elevator.CURRENT_FLOOR <= 12 && (uplistSize != 0 || elevator.CURRENT_FLOOR < highestFloor))
    {
        if (downlistSize == 0 && uplistSize == 0)
        {
            return;
        }
        // tìm kiếm tầng tiếp theo cần lên;
        int next_floor = findNextFloor_Up(uplist, elevator.CURRENT_FLOOR - 1, uplistSize);
        // tìm kiếm tầng tiếp theo cần đón khách;
        int pickup_floor = findNextFloor_Up(waitUp, elevator.CURRENT_FLOOR - 1, waitUpSize);

        cout << "The elevator is at floor: " << elevator.CURRENT_FLOOR;
        Sleep(500);
        if (elevator.CURRENT_FLOOR != 12)
        {
            printf("\33[2K\r");
        }
        if (elevator.CURRENT_FLOOR == next_floor && elevator.CURRENT_FLOOR == pickup_floor)
        {
            if (elevator.CURRENT_FLOOR == 12)
            {
                printf("\33[2K\r");
            }
            cout << "The elevator is at floor: " << elevator.CURRENT_FLOOR << " - USER IN, USER OUT\n";
            // trường hợp có nhiều người cùng ra khỏi 1 tầng thì xóa tất cả node
            while (findPositionByValue(uplist, next_floor, uplistSize) != -1)
            {
                uplist = deleteFloor(uplist, findPositionByValue(uplist, next_floor, uplistSize), uplistSize);
            }
            while (findPositionByValue(waitUp, pickup_floor, waitUpSize) != -1)
            {
                waitUp = deleteFloor(waitUp, findPositionByValue(waitUp, pickup_floor, waitUpSize), waitUpSize);
            }
            cout << "\t\t----\n";
            elevator.CURRENT_FLOOR++;
            continue;
        }
        if (elevator.CURRENT_FLOOR == pickup_floor)
        {
            if (elevator.CURRENT_FLOOR == 12)
            {
                printf("\33[2K\r");
            }
            cout << "The elevator is at floor: " << elevator.CURRENT_FLOOR << " - USER IN\n";
            while (findPositionByValue(waitUp, pickup_floor, waitUpSize) != -1)
            {
                waitUp = deleteFloor(waitUp, findPositionByValue(waitUp, pickup_floor, waitUpSize), waitUpSize);
            }
            cout << "\t\t----\n";
        }
        if (elevator.CURRENT_FLOOR == next_floor)
        {
            if (elevator.CURRENT_FLOOR == 12)
            {
                printf("\33[2K\r");
            }
            cout << "The elevator is at floor: " << elevator.CURRENT_FLOOR << " - USER OUT\n";
            while (findPositionByValue(uplist, next_floor, uplistSize) != -1)
            {
                uplist = deleteFloor(uplist, findPositionByValue(uplist, next_floor, uplistSize), uplistSize);
            }
            cout << "\t\t----\n";
        }
        elevator.CURRENT_FLOOR++;
    }
    if (elevator.CURRENT_FLOOR > 12)
    {
        elevator.CURRENT_FLOOR = 12;
    }
    if (downlistSize != 0 || uplistSize != 0)
    {
        solveDown(waitUp, waitDown, uplist, downlist, elevator);
    }
}
void solveDown(FLOOR *waitUp, FLOOR *waitDown, FLOOR *uplist, FLOOR *downlist, ELEVATOR &elevator)
{
    elevator.STATE = "GOINGDOWN";
    cout << "\n---------THE ELEVATOR IS GOING DOWN---------" << endl;
    while (elevator.CURRENT_FLOOR >= 1 && (downlistSize != 0 || elevator.CURRENT_FLOOR > lowestFloor))
    {
        if (downlistSize == 0 && uplistSize == 0)
        {
            return;
        }
        // tìm kiếm tầng tiếp theo cần xuống
        int next_floor = findNextFloor_Down(downlist, elevator.CURRENT_FLOOR + 1, downlistSize);
        // tìm kiếm tầng tiếp theo cần đón khách
        int pickup_floor = findNextFloor_Down(waitDown, elevator.CURRENT_FLOOR + 1, waitDownSize);

        cout << "The elevator is at floor: " << elevator.CURRENT_FLOOR;
        Sleep(400);
        if (elevator.CURRENT_FLOOR != 1)
        {
            printf("\33[2K\r");
        }

        if (elevator.CURRENT_FLOOR == next_floor && elevator.CURRENT_FLOOR == pickup_floor)
        {
            if (elevator.CURRENT_FLOOR == 1)
            {
                printf("\33[2K\r");
            }
            cout << "The elevator is at floor: " << elevator.CURRENT_FLOOR << " - USER IN, USER OUT\n";
            while (findPositionByValue(downlist, next_floor, downlistSize) != -1)
            {
                downlist = deleteFloor(downlist, findPositionByValue(downlist, next_floor, downlistSize), downlistSize);
            }
            while (findPositionByValue(waitDown, pickup_floor, waitDownSize) != -1)
            {
                waitDown = deleteFloor(waitDown, findPositionByValue(waitDown, pickup_floor, waitDownSize), waitDownSize);
            }
            cout << "\t\t----\n";
            elevator.CURRENT_FLOOR--;
            continue;
        }
        if (elevator.CURRENT_FLOOR == pickup_floor)
        {
            if (elevator.CURRENT_FLOOR == 1)
            {
                printf("\33[2K\r");
            }
            cout << "The elevator is at floor: " << elevator.CURRENT_FLOOR << " - USER IN\n";
            while (findPositionByValue(waitDown, pickup_floor, waitDownSize) != -1)
            {
                waitDown = deleteFloor(waitDown, findPositionByValue(waitDown, pickup_floor, waitDownSize), waitDownSize);
            }
            cout << "\t\t----\n";
        }
        if (elevator.CURRENT_FLOOR == next_floor)
        {
            if (elevator.CURRENT_FLOOR == 1)
            {
                printf("\33[2K\r");
            }
            cout << "The elevator is at floor: " << elevator.CURRENT_FLOOR << " - USER OUT\n";
            while (findPositionByValue(downlist, next_floor, downlistSize) != -1)
            {
                downlist = deleteFloor(downlist, findPositionByValue(downlist, next_floor, downlistSize), downlistSize);
            }
            cout << "\t\t----\n";
        }
        elevator.CURRENT_FLOOR--;
    }
    if (elevator.CURRENT_FLOOR < 1)
    {
        elevator.CURRENT_FLOOR = 1;
    }
    if (downlistSize != 0 || uplistSize != 0)
    {
        solveUp(waitUp, waitDown, uplist, downlist, elevator);
    }
}
int findPositionByValue(FLOOR *list, int value, int size)
{
    FLOOR *browse = list;
    if (size == 0)
    {
        return -1;
    }
    int i = 0;
    while (browse != nullptr)
    {
        if (browse->floor_number == value)
        {
            break;
        }
        browse = browse->next;
        i++;
    }
    if (browse == nullptr)
    {
        return -1;
    }
    return i;
}
FLOOR *deleteFloor(FLOOR *list, int position, int &size)
{
    FLOOR *browse = list;
    if (position == 0)
    {
        browse = browse->next;
        delete (list);
        size--;
        return browse;
    }
    else if (position == size - 1)
    {
        while (browse->next->next != nullptr)
        {
            browse = browse->next;
        }
        delete (browse->next);
        size--;
        browse->next = nullptr;
        return list;
    }
    else
    {
        for (int i = 0; i < position - 1; i++)
        {
            browse = browse->next;
        }
        FLOOR *temp = browse->next;
        browse->next = browse->next->next;
        delete (temp);
        size--;
        return list;
    }
    return list;
}
FLOOR *createFloor(int x)
{
    FLOOR *temp = new FLOOR{x, nullptr};
    return temp;
}
FLOOR *insertTail(FLOOR *list, int x, int &size)
{
    if (size == 0)
    {
        list = createFloor(x);
        return list;
    }
    FLOOR *browse = list;
    while (browse->next != nullptr)
    {
        browse = browse->next;
    }
    FLOOR *temp = new FLOOR{x, nullptr};
    browse->next = temp;
    return list;
}
int findNextFloor_Up(FLOOR *uplist, int current_floor, int &size)
{
    FLOOR *browse = uplist;
    int temp = -1;
    if (size == 0)
    {
        return -1;
    }

    while (browse != nullptr)
    {
        if (browse->floor_number > current_floor)
        {
            temp = browse->floor_number;
            break;
        }
        browse = browse->next;
    }
    while (browse != nullptr)
    {
        if (browse->floor_number > current_floor && browse->floor_number < temp)
        {
            temp = browse->floor_number;
        }
        browse = browse->next;
    }
    return temp;
}
int findNextFloor_Down(FLOOR *downlist, int current_floor, int &size)
{
    FLOOR *browse = downlist;
    int temp = -1;
    if (size == 0)
    {
        return -1;
    }

    while (browse != nullptr)
    {
        if (browse->floor_number < current_floor)
        {
            temp = browse->floor_number;
            break;
        }
        browse = browse->next;
    }
    while (browse != nullptr)
    {
        if (browse->floor_number < current_floor && browse->floor_number > temp)
        {
            temp = browse->floor_number;
        }
        browse = browse->next;
    }
    return temp;
}