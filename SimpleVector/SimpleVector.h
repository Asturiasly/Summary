#pragma once
#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <iostream>

#include "array_ptr.h"

class ReserveProxyObj {
public:
    explicit ReserveProxyObj(size_t capacity_to_reserve) :proxy_capacity_(capacity_to_reserve) {}

    size_t GetCapacity()
    {
        return proxy_capacity_;
    }

private:
    size_t proxy_capacity_;
};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}

template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;
    using Array = ArrayPtr<Type>;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size) : SimpleVector(size, std::move(Type{})) {}

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, Type value) : data_(size), size_(size), capacity_(size)
    {
        std::fill(begin(), end(), value);
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init) : data_(init.size()), size_(init.size()), capacity_(init.size())
    {
        std::move(init.begin(), init.end(), begin());
    }
    // Конструктор копирования
    SimpleVector(const SimpleVector& other) : data_(other.capacity_), size_(other.size_), capacity_(other.capacity_)
    {
        std::copy(other.begin(), other.end(), begin());
    }
    // Оператор присваивания для копирования
    SimpleVector& operator=(const SimpleVector& rhs) {
        if (this != &rhs) {
            SimpleVector tmp(rhs);
            swap(tmp);
        }
        return *this;
    }

    // Конструктор перемещения
    SimpleVector(SimpleVector&& moved) noexcept {
        SimpleVector tmp;

        Array tmp_data(moved.GetSize());
        std::move(moved.begin(), moved.end(), tmp_data.Get());

        tmp.size_ = std::exchange(moved.size_, 0);
        tmp.capacity_ = std::exchange(moved.capacity_, 0);
        tmp.data_ = std::move(tmp_data);

        swap(tmp);
    }

    // Оператор присваивания для перемещения
    SimpleVector& operator=(SimpleVector&& rhs) noexcept
    {
        if (this != &rhs)
        {
            data_ = std::move(rhs.data_);
            size_ = std::exchange(rhs.size_, 0);
            capacity_ = std::exchange(rhs.capacity_, 0);

        }
        return *this;
    }

    SimpleVector(ReserveProxyObj capacity_to_reserve) {
        Reserve(capacity_to_reserve.GetCapacity());
    }

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept {
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        // Напишите тело самостоятельно
        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        return size_ == 0;
    }

     //Добавляет элемент в конец вектора
     //При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(const Type& value)
    {
        const auto pos = size_;
        if (size_ == capacity_)
        {
            Resize(capacity_ + 1);
        }
        else
        {
            ++size_;
        }
        data_[pos] = value;
    }

    void PushBack(Type&& value)
    {
        const auto pos = size_;
        if (size_ == capacity_)
        {
            Resize(capacity_ + 1);
        }
        else
        {
            ++size_;
        }
        data_[pos] = std::move(value);
    }

    Iterator Insert(ConstIterator pos, const Type& value)
    {
        assert(begin() <= pos && pos <= end());
        Iterator position = const_cast<Iterator>(pos);
        size_t index = std::distance(begin(), position);

        if (capacity_ == size_)
        {
            size_t new_capacity;
            capacity_ == 0 ? new_capacity = 1 : new_capacity = capacity_ * 2;
            Array temp(new_capacity);
            std::move(begin(), begin() + index, temp.Get());
            temp[index] = std::move(value);
            std::move(begin() + index, end(), temp.Get() + index + 1);
            capacity_ = new_capacity;
            size_ = GetSize() + 1;
            data_.swap(temp);
            return begin() + index;
        }
        else
        {
            std::move_backward(position, end(), std::next(end()));
            *position = std::move(value);
            ++size_;
            return position;
        }
    }

    Iterator Insert(ConstIterator pos, Type&& value) {
        assert(begin() <= pos && pos <= end());
        Iterator position = const_cast<Iterator>(pos);
        size_t index = std::distance(begin(), position);

        if (capacity_ == size_)
        {
            size_t new_capacity;
            capacity_ == 0 ? new_capacity = 1 : new_capacity = capacity_ * 2;
            Array temp(new_capacity);
            std::move(begin(), begin() + index, temp.Get());
            temp[index] = std::move(value);
            std::move(begin() + index, end(), temp.Get() + index + 1);
            capacity_ = new_capacity;
            size_ = GetSize() + 1;
            data_.swap(temp);
            return begin() + index;
        }
        else
        {
            std::move_backward(position, end(), std::next(end()));
            *position = std::move(value);
            ++size_;
            return position;
        }
    }

    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept {
        assert(size_ != 0);
        --size_;
    }

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
        assert(begin() <= pos && pos <= end());
        assert(size_ != 0);
        Iterator position = const_cast<Iterator>(pos);
        std::move(std::next(position), end(), position);
        --size_;
        return position;
    }

    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept {
        data_.swap(other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        assert(!(index > size_));
        return data_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        assert(!(index > size_));
       return data_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if (index >= size_)
            throw std::out_of_range("simple_vector subscript out of range");
        else
            return data_[index];
        // Напишите тело самостоятельно
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if (index >= size_)
            throw std::out_of_range("simple_vector subscript out of range");
        else
            return data_[index];
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        size_ = 0;
    }

    /* Изменяет размер массива.
     При увеличении размера новые элементы получают значение по умолчанию для типа Type*/
    void Resize(size_t new_size)
    {
        if (new_size > size_ && new_size <= capacity_)
        {
            for (size_t i = size_; i < new_size; ++i)
            {
                data_[i] = std::move(Type{});
            }
        }

        if (new_size > capacity_)
        {
            capacity_ = new_size;
            ArrayPtr<Type> temp(new_size);
            std::move(begin(), end(), temp.Get());

            for (size_t i = size_; i < new_size; ++i)
            {
                temp[i] = std::move(Type{});
            }

            data_ = std::move(temp);
        }
        size_ = new_size;
    }

    void Reserve(size_t new_capacity) {
        if (new_capacity <= GetCapacity()) {
            return;
        }

        Array temp(new_capacity);
        std::move(begin(), end(), temp.Get());
        data_ = std::move(temp);

        capacity_ = new_capacity;
    }

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        return data_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        return (data_.Get() + GetSize());
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        return cbegin();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        return cend();
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        return data_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        return (data_.Get() + GetSize());
    }
    private:
        ArrayPtr<Type> data_;
        size_t size_ = 0;
        size_t capacity_ = 0;
};

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return !(lhs == rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return !(rhs < lhs);
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return rhs < lhs;
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return !(lhs < rhs);
}
