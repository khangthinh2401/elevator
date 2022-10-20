#include <iostream>
#include <algorithm>
#include <string>
#include <windows.h>
#include <queue>

using namespace std;
struct GUEST
{
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
int findNextFloor_Up(FLOOR *uplist, int current_floor);
int findNextFloor_Down(FLOOR *downlist, int current_floor);
void solveUp(queue<GUEST> &q, FLOOR *uplist, FLOOR *downlist, ELEVATOR &elevator);
void solveDown(queue<GUEST> &q, FLOOR *uplist, FLOOR *downlist, ELEVATOR &elevator);
int uplistSize = 0;
int downlistSize = 0;
int highestFloor = 1;
int lowestFloor = 12;

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    system("cls");
    // printf("\e[?25l");
    // printf("\33[2K\r");

    ELEVATOR elevator = {5, "NEUTRAL", 0, 0, 0};
    FLOOR *uplist = new FLOOR;
    FLOOR *downlist = new FLOOR;
    queue<GUEST> q;
    queue<GUEST> waitToGoUp;
    queue<GUEST> waitToGoDown;

    int n;
    cout << "Nhập số người sử dụng thang máy: ";
    cin >> n;
    cout << "=================================================\n";
    int current_floor_temp, floor_temp;
    string flag_temp;
    for (int i = 0; i < n; i++)
    {
        cout << "Khách số " << i + 1 << endl;
        cout << "Nhấn nút (up/down): ";
        cin >> flag_temp;
        cout << "Tầng hiện tại:      ";
        cin >> current_floor_temp;
        cout << "Tầng muốn đến:      ";
        cin >> floor_temp;
        GUEST guest = {floor_temp, flag_temp};
        if (flag_temp == "up")
        {
            uplist = insertTail(uplist, floor_temp, uplistSize);
            uplistSize++;
            if (current_floor_temp < lowestFloor)
            {
                lowestFloor = current_floor_temp;
            }
        }
        else
        {
            downlist = insertTail(downlist, floor_temp, downlistSize);
            downlistSize++;
            if (current_floor_temp > highestFloor)
            {
                highestFloor = current_floor_temp;
            }
        }
        cout << endl;
        q.push(guest);
    }

    // check xem người đầu tiên bấm đi lên / đi xuống
    // đi lên
    if (q.front().flag == "up")
    {
        printf("\e[?25l");
        solveUp(waitToGoDown, uplist, downlist, elevator);
    }
    // đi xuống
    else
    {
        printf("\e[?25l");
        solveDown(waitToGoUp, uplist, downlist, elevator);
    }

    return 0;
}

void solveUp(queue<GUEST> &q, FLOOR *uplist, FLOOR *downlist, ELEVATOR &elevator)
{
    elevator.STATE = "GOINGUP";
    cout << "\n----------THANG ĐANG ĐI LÊN----------" << endl;
    while (elevator.CURRENT_FLOOR <= 12 && (uplistSize != 0 || elevator.CURRENT_FLOOR < highestFloor))
    {
        // tìm kiếm tầng tiếp theo cần lên;
        int next_floor = findNextFloor_Up(uplist, elevator.CURRENT_FLOOR - 1);
        // tìm kiếm tầng tiếp theo cần đón khách;
        int pickup_floor = 0;

        cout << "Thang máy đang ở tầng: " << elevator.CURRENT_FLOOR;
        Sleep(500);
        if (elevator.CURRENT_FLOOR != 12)
        {
            printf("\33[2K\r");
        }
        if (elevator.CURRENT_FLOOR == next_floor)
        {
            if (elevator.CURRENT_FLOOR == 12)
            {
                printf("\33[2K\r");
            }
            cout << "Thang máy đang ở tầng: " << elevator.CURRENT_FLOOR << " - CÓ KHÁCH ĐI RA\n";
            // trường hợp có nhiều người cùng ra khỏi 1 tầng thì xóa tất cả node
            while (findPositionByValue(uplist, next_floor, uplistSize) != -1)
            {
                uplist = deleteFloor(uplist, findPositionByValue(uplist, next_floor, uplistSize), uplistSize);
            }
            cout << "\t\t----\n";
        }
        elevator.CURRENT_FLOOR++;
    }
    elevator.CURRENT_FLOOR = 12;
    if (uplistSize != 0 || downlistSize != 0)
    {
        solveDown(q, uplist, downlist, elevator);
    }
}
void solveDown(queue<GUEST> &q, FLOOR *uplist, FLOOR *downlist, ELEVATOR &elevator)
{
    elevator.STATE = "GOINGDOWN";
    cout << "\n--------THANG ĐANG ĐI XUỐNG--------" << endl;
    while (elevator.CURRENT_FLOOR >= 1 && (downlistSize != 0 || elevator.CURRENT_FLOOR > lowestFloor))
    {
        // tìm kiếm tầng tiếp theo cần lên
        int next_floor = findNextFloor_Down(downlist, elevator.CURRENT_FLOOR + 1);
        // tìm kiếm tầng tiếp theo cần đón khách
        int pickup_floor = 0;

        cout << "Thang máy đang ở tầng: " << elevator.CURRENT_FLOOR;
        Sleep(400);
        if (elevator.CURRENT_FLOOR != 1)
        {
            printf("\33[2K\r");
        }
        if (elevator.CURRENT_FLOOR == next_floor)
        {
            if (elevator.CURRENT_FLOOR == 1)
            {
                printf("\33[2K\r");
            }
            cout << "Thang máy đang ở tầng: " << elevator.CURRENT_FLOOR << " - CÓ KHÁCH ĐI RA\n";
            downlist = deleteFloor(downlist, findPositionByValue(downlist, next_floor, downlistSize), downlistSize);
            cout << "\t\t----\n";
        }
        elevator.CURRENT_FLOOR--;
    }
    elevator.CURRENT_FLOOR = 1;
    if (downlistSize != 0 || uplistSize != 0)
    {
        solveUp(q, uplist, downlist, elevator);
    }
}
int findPositionByValue(FLOOR *list, int value, int size)
{
    FLOOR *browse = list;
    int i = 0;
    while (browse != nullptr)
    {
        if (browse->floor_number == value)
        {
            break;
        }
        i++;
        browse = browse->next;
    }
    if (i == size)
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
int findNextFloor_Up(FLOOR *uplist, int current_floor)
{
    FLOOR *browse = uplist;
    int temp;
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
int findNextFloor_Down(FLOOR *downlist, int current_floor)
{
    FLOOR *browse = downlist;
    int temp;
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