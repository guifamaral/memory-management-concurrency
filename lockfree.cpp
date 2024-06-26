#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

template<typename T>
class lockfreestack
{
    public:
        void push(T value)
        {
            Node* newnode = new Node(std::move(value));
            newnode -> next = head.load();
            while(!head.compare_exchange_weak(newnode -> next, newnode));
        }

        std::shared_ptr<T> pop()
        {
            Node* oldhead = head.load();
            while(oldhead && !head.compare_exchange_weak(oldhead, oldhead -> next));
            return oldhead ? std::make_shared<T>(std::move(oldhead->value)) : std::shared_ptr<T>();
        }
    private:
        struct Node
        {
            T value;
            Node* next;
            Node(T val) : value(std::move(val)), next(nullptr) {}
        };
        std::atomic<Node*> head{nullptr};
};

int main()
{
    lockfreestack<int> stack;
    std::thread p1();
    std::thread p2();
    std::thread c1();
    std::thread c2();

    p1.join();
    p2.join();
    c1.join();
    c2.join();

    return 0;
}