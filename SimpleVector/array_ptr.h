#include <cassert>
#include <cstdlib>

template <typename Type>
class ArrayPtr
{
public:
    // �������������� ArrayPtr ������� ����������
    ArrayPtr() = default;

    // ������ � ���� ������ �� size ��������� ���� Type.
    // ���� size == 0, ���� raw_ptr_ ������ ���� ����� nullptr
    explicit ArrayPtr(size_t size)
    {
        if (size != 0)
        {
            raw_ptr_ = new Type[size];
        }
        else
        {
            raw_ptr_ = nullptr;
        }
    }

    // ����������� �� ������ ���������, ��������� ����� ������� � ���� ����
    // nullptr
    explicit ArrayPtr(Type* raw_ptr) noexcept
    {
        raw_ptr_ = raw_ptr;
    }

    // ��������� �����������
    ArrayPtr(const ArrayPtr&) = delete;

    // ��������� ������������
    ArrayPtr& operator=(const ArrayPtr&) = delete;

    ArrayPtr(ArrayPtr&& other)
    {
        raw_ptr_ = other.raw_ptr_;
        other.raw_ptr_ = nullptr;
    }

    ~ArrayPtr()
    {
        delete[] raw_ptr_;
    }


    ArrayPtr& operator=(ArrayPtr&& other)
    {
        if (this == &other)
        {
            return *this;
        }

        raw_ptr_ = other.raw_ptr_;
        other.raw_ptr_ = nullptr;
        return *this;
    }

    // ���������� ��������� �������� � ������, ���������� �������� ������ �������
    // ����� ������ ������ ��������� �� ������ ������ ����������
    [[nodiscard]] Type* Release() noexcept
    {
        auto ptr = raw_ptr_;
        raw_ptr_ = nullptr;
        return ptr;
    }

    // ���������� ������ �� ������� ������� � �������� index
    Type& operator[](size_t index) noexcept
    {
        return *(raw_ptr_ + index);
    }

    // ���������� ����������� ������ �� ������� ������� � �������� index
    const Type& operator[](size_t index) const noexcept
    {
        return *(raw_ptr_ + index);
    }

    // ���������� true, ���� ��������� ���������, � false � ��������� ������
    explicit operator bool() const
    {
        return raw_ptr_;
    }

    // ���������� �������� ������ ���������, ��������� ����� ������ �������
    Type* Get() const noexcept
    {
        return raw_ptr_;
    }

    // ������������ ��������� ��������� �� ������ � �������� other
    void swap(ArrayPtr& other) noexcept
    {
        auto b = other.raw_ptr_;
        other.raw_ptr_ = raw_ptr_;
        raw_ptr_ = b;
    }

private:
    Type* raw_ptr_ = nullptr;
};