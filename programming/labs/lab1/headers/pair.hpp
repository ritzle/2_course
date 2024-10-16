#ifndef PAIR_H
#define PAIR_H

template <typename Key, typename Value>
class Pair {
 public:
  Key key;
  Value value;
  Pair* next;  // указатель на следующий элемент в случае коллизии

  // Конструктор
  Pair(const Key& k, const Value& v) : key(k), value(v), next(nullptr) {}

  // Конструктор копирования
  Pair(const Pair& other) : key(other.key), value(other.value), next(nullptr) {
    if (other.next) {
      next =
          new Pair(*other.next);  // Рекурсивное копирование следующего элемента
    }
  }

  // Деструктор
  ~Pair() {
    delete next;  // Освобождаем память для следующего элемента
  }

  // Оператор присваивания
  Pair& operator=(const Pair& other) {
    if (this != &other) {  // Проверка на самоприсваивание
      key = other.key;
      value = other.value;

      // Освобождаем память для текущего next
      delete next;

      // Копируем next
      if (other.next) {
        next = new Pair(
            *other.next);  // Рекурсивное копирование следующего элемента
      } else {
        next = nullptr;  // Если у другого нет next, устанавливаем null
      }
    }
    return *this;
  }
};

#endif  // PAIR_H