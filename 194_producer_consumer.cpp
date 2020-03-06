//
// Created by Zeno on 2020/3/8.
//

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue> // C++ STL���е������������̰߳�ȫ

using namespace std;

// ����������һ����Ʒ��֪ͨ����������һ�����������ˣ���������֪ͨ�����߽�������

std::mutex mtx; // ���廥���������̼߳�Ļ������
std::condition_variable cv; // �����������������̼߳��ͬ��ͨ�Ų���

class Queue
{
public:
    void put(int val) // ������Ʒ
    {
        //lock_guard<std::mutex> guard(mtx);
        unique_lock<std::mutex> lck(mtx);
        while (!que.empty())
        {
            // que��Ϊ�գ�������Ӧ��֪ͨ������ȥ���ѣ��������ˣ��ټ�������
            // �������߳̽���ȴ�״̬�����Ұ�mtx�������ͷŵ�
            cv.wait(lck);
        }
        que.push(val);

        cv.notify_all(); // ֪ͨ�������е��̣߳���������һ����Ʒ�����ǸϽ����Ѱ�
        // �����̵߳õ���֪ͨ���ͻ�ӵȴ�״̬��������״̬�����ǻ���Ҫ��ȡ���������ܼ�������
        cout << "������ ������" << val << "����Ʒ" << endl;
    }

    int get() //������Ʒ
    {
        //lock_guard<std::mutex> guard(mtx);
        unique_lock<std::mutex> lck(mtx);
        while(que.empty())
        {
            // �������̷߳���que�ǿյģ�֪ͨ�������߳���������Ʒ
            // ����ȴ�״̬���ѻ�����mutex�ͷ�
            cv.wait(lck);
        }
        int val = que.front();
        que.pop();
        cv.notify_all(); // ֪ͨ�����̣߳����������ˣ��Ͻ�������
        cout << "������ ���ѣ�" << val << "����Ʒ" << endl;
        return val;
    }

private:
    queue<int> que;
};

void producer(Queue* que) // �������߳�
{
    for(int i = 1; i <= 10; ++i)
    {
        que->put(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void consumer(Queue* que) // �������߳�
{
    for(int i = 1; i <= 10; ++i)
    {
        que->get();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main()
{
    Queue que; //�����̹߳���Ķ���

    std::thread t1(producer, &que);
    std::thread t2(consumer, &que);

    t1.join();
    t2.join();

    return 0;
}
