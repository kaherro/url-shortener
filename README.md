# URL Shortener Microservice

Микросервис для сокращения ссылок на C++ с использованием Crow и SQLite.

## Особенности

- Генерация коротких ссылок в формате base62 (A-Z, a-z, 0-9)
- Настраиваемый срок действия ссылки (expiration date)
- Редирект на оригинальный URL
- Простая HTTP-интеграция

## Технологии

- **C++20** 
- **Crow** 
- **SQLite3**
- **Cmake**

### Структура проекта
```
project/
├── src/
│   ├── main.cpp           # Основной файл с API
│   ├── database.cpp       # Работа с БД (SQLite3)
│   └── base62.cpp         # Конвертация base10 ↔ base62
├── include/
│   ├── database.h
│   └── base62.h
├── CMakeLists.txt         # Конфигурация сборки
├── test.db                # Пример базы данных
└── README.md              # Документация проекта
```

## Установка

### Требования
- Компилятор с поддержкой C++20 (g++ 10+)
- Crow
- SQLite3

### Сборка и запуск
```bash
mkdir /build
cd build
cmake ..
make
./main
```

# API
## Создать короткую ссылку: 
```bash
curl -X POST http://localhost:18080/short -d '{"url": "https://google.com", "expiration_date" : "2025-05-09 14:51:51"}'
```

Ответ: 
```bash
{"short_url": "http://localhost:18080/aBc12"}
```
Параметр "url" указывать обязательно, "expiration_date" опционален (но если не указать, то ссылка будет считаться бессрочной). 
## Перейти по короткой ссылке:
```bash
curl http://localhost:18080/aBc12
```
→ 302 Redirect на оригинальный URL
