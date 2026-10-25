#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <utility>

// Интерфейс для текстового формата
class PrintableAsText
{
public:
    virtual ~PrintableAsText() = default;
    virtual std::string printAsText() const = 0;
};

// Интерфейс для HTML
class PrintableAsHTML
{
public:
    virtual ~PrintableAsHTML() = default;
    virtual std::string printAsHTML() const = 0;
};

// Интерфейс для JSON
class PrintableAsJSON
{
public:
    virtual ~PrintableAsJSON() = default;
    virtual std::string printAsJSON() const = 0;
};

// Класс для хранения данных
class Data
{
public:
    explicit Data(std::string data)
        : data_(std::move(data))
    {
    }

protected:
    const std::string& getData() const
    {
        return data_;
    }

private:
    std::string data_;
};

// Данные в текстовом формате
class TextData : public Data, public PrintableAsText
{
public:
    using Data::Data;

    std::string printAsText() const override
    {
        return getData();
    }
};

// Данные в формате HTML
class HTMLData : public Data, public PrintableAsHTML
{
public:
    using Data::Data;

    std::string printAsHTML() const override
    {
        return "<html>" + getData() + "</html>";
    }
};

// Данные в формате JSON
class JSONData : public Data, public PrintableAsJSON
{
public:
    using Data::Data;

    std::string printAsJSON() const override
    {
        return "{ \"data\": \"" + getData() + "\"}";
    }
};

// Универсальная функция сохранения
template <typename T>
void saveTo(std::ofstream& file, const T& printable)
{
    file << printable;

    if (!file)
    {
        throw std::runtime_error("Error writing to file!");
    }
}

// Перегрузка оператора вывода для текстового формата
std::ostream& operator<<(std::ostream& out,
    const PrintableAsText& printable)
{
    return out << printable.printAsText();
}

// Перегрузка оператора вывода для HTML
std::ostream& operator<<(std::ostream& out,
    const PrintableAsHTML& printable)
{
    return out << printable.printAsHTML();
}

// Перегрузка оператора вывода для JSON
std::ostream& operator<<(std::ostream& out,
    const PrintableAsJSON& printable)
{
    return out << printable.printAsJSON();
}

// Сохранение в текстовом формате
void saveToAsText(std::ofstream& file,
    const PrintableAsText& printable)
{
    saveTo(file, printable);
}

// Сохранение в HTML
void saveToAsHTML(std::ofstream& file,
    const PrintableAsHTML& printable)
{
    saveTo(file, printable);
}

// Сохранение в JSON
void saveToAsJSON(std::ofstream& file,
    const PrintableAsJSON& printable)
{
    saveTo(file, printable);
}

int main()
{
    try
    {
        TextData text("Hello, world!");
        HTMLData html("Hello, world!");
        JSONData json("Hello, world!");

        {
            std::ofstream file("text.txt");
            if (!file)
                throw std::runtime_error("Cannot open text.txt");

            saveToAsText(file, text);
        }

        {
            std::ofstream file("data.html");
            if (!file)
                throw std::runtime_error("Cannot open data.html");

            saveToAsHTML(file, html);
        }

        {
            std::ofstream file("data.json");
            if (!file)
                throw std::runtime_error("Cannot open data.json");

            saveToAsJSON(file, json);
        }

        std::cout << "Files saved successfully!" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}