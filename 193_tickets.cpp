//
// Created by Zeno on 2020/3/7.
//

// ģ�⳵վ����������Ʊ�ĳ���

#include <iostream>
#include <thread>
#include <mutex>
#include <list>

using namespace std;

int count = 100; // ��վ��һ����Ʊ������������һ����Ʊ
std::mutex mtx; // ȫ�ֵ�һ�ѻ�����

// ģ����Ʊ���̺߳���
void sellTicket(int index)
{
    while(count > 0)
    {
        //mtx.lock();
        {
            lock_guard<std::mutex> lock(mtx);
            //unique_lock<std::mutex> lock(mtx);
            if (count > 0) {
                // �ٽ�������� Ӧ����ԭ�Ӳ������̼߳令�ֲ���
                cout << "����" << index << "������" << count << "��Ʊ" << endl;
                //cout << count << endl;
                count--;
            }
        }
        //mtx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}


int main()
{
    list<std::thread> tlist;
    for(int i = 1; i <=3; ++i)
    {
        tlist.push_back(std::thread(sellTicket, i));
    }

    for(std::thread &t : tlist)
    {
        t.join();
    }

    cout << "���д�����Ʊ����" << endl;

    return 0;
}
