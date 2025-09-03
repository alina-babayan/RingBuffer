#include <iostream>
#include <memory>


template <typename T>
class RingBuffer
{
    private:
        std::size_t len{};
        std::size_t capacity{};
        std::size_t head{};
        std::size_t tail{};
        std::unique_ptr<T[]> arr;


        std::size_t next(std::size_t i) { return (i + 1) % capacity; };
        std::size_t prev(std::size_t i) { return (i + capacity - 1)% capacity; };
        void resize();
        
    public:
        RingBuffer(long long cap = 0);
        void push_back(T val);
        void push_front(T val);
        void pop_back();
        void pop_front();
        std::size_t size() const { return len;};
        bool empty() const {return len == 0;};
        T& front();
        T& back();
};

template <typename T>
RingBuffer<T>::RingBuffer(long long cap)
{
    if(cap > 0)
    {
        arr = std::make_unique<T[]>(cap);
    }
    capacity = cap;
}

template <typename T>
void RingBuffer<T>:: resize()
{
    auto newcap = capacity == 0 ? 1 : capacity * 2;
    auto newdata = std::make_unique<T[]>(newcap);

    std::size_t ptr = head;
    for(std::size_t i{}; i < len; ++i)
    {
        newdata[i] = arr[ptr];
        ptr = next(ptr);
    }
    arr = std::move(newdata);
    capacity = std::move(newcap);
    head = 0;
    tail = len;
}

template <typename T>
void RingBuffer<T>::push_back(T val)
{
    if(len == capacity) resize();
    arr[tail] = val;
    tail = next(tail);
    len++;
}

template <typename T>
void RingBuffer<T>::push_front(T val)
{
    if(len == capacity) resize();
    head = prev(head);
    arr[head] = val;
    len++;
}

template <typename T>
void RingBuffer<T>::pop_back()
{
    tail = prev(tail);
    len--;
}

template <typename T>
void RingBuffer<T>::pop_front()
{
    head = next(head);
    len--;
}

template <typename T>
T& RingBuffer<T>::front()
{
    return arr[head];
}

template <typename T>
T& RingBuffer<T>::back()
{
    return arr[prev(tail)];
}





int main()
{
    RingBuffer<int> rb(3);  

    rb.push_back(10);
    rb.push_back(20);
    rb.push_back(30);

    std::cout << "Front: " << rb.front() << "\n"; 
    std::cout << "Back: "  << rb.back()  << "\n"; 

    rb.pop_front();
    std::cout << "After pop_front, new front: " << rb.front() << "\n"; 

    rb.push_front(5);
    std::cout << "After push_front(5), new front: " << rb.front() << "\n"; 

    rb.push_back(40);
    rb.push_back(50);

    std::cout << "After resize, size = " << rb.size() << "\n";
    std::cout << "Front: " << rb.front() << ", Back: " << rb.back() << "\n";

    rb.pop_back();
    std::cout << "After pop_back, new back: " << rb.back() << "\n";

    return 0;
}