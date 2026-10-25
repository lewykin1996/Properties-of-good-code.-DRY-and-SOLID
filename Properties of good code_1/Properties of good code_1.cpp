#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

// Интерфейс для текстового формата
class TextPrintable
{
public:
    virtual ~TextPrintable() = default;
    virtual std::string print() const = 0;
};

// Интерфейс для HTML
class HTMLPrintable
{
public:
    virtual ~HTMLPrintable() = default;
    virtual std::string print() const = 0;
};

// Интерфейс для JSON
class JSONPrintable
{
public:
    virtual ~JSONPrintable() = default;
    virtual std::string print() const = 0;
};

// Базовый класс с данными
class Data
{
public:
    explicit Data(std::string data)
        : data_(std::move(data))
    {
    }

    const std::string& getData() const
    {
        return data_;
    }

private:
    std::string data_;
};

// Данные в текстовом формате
class TextData : public Data, public TextPrintable
{
public:
    using Data::Data;

    std::string print() const override
    {
        return getData();
    }
};

// Данные в HTML
class HTMLData : public Data, public HTMLPrintable
{
public:
    using Data::Data;

    std::string print() const override
    {
        return "<html>" + getData() + "</html>";
    }
};

// Данные в JSON
class JSONData : public Data, public JSONPrintable
{
public:
    using Data::Data;

    std::string print() const override
    {
        return "{ \"data\": \"" + getData() + "\"}";
    }
};

// Универсальная функция сохранения.
// Она не знает, в каком формате представлены данные.
template <typename T>
void saveTo(std::ofstream& file, const T& printable)
{
    file << printable.print();

    if (!file)
    {
        throw std::runtime_error("Failed to write to file!");
    }
}

// Сохранение в текстовом формате
void saveToAsText(std::ofstream& file,
    const TextPrintable& printable)
{
    saveTo(file, printable);
}

// Сохранение в HTML
void saveToAsHTML(std::ofstream& file,
    const HTMLPrintable& printable)
{
    saveTo(file, printable);
}

// Сохранение в JSON
void saveToAsJSON(std::ofstream& file,
    const JSONPrintable& printable)
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
            {
                throw std::runtime_error("Cannot open text.txt");
            }
            saveToAsText(file, text);
        }

        {
            std::ofstream file("data.html");
            if (!file)
            {
                throw std::runtime_error("Cannot open data.html");
            }
            saveToAsHTML(file, html);
        }

        {
            std::ofstream file("data.json");
            if (!file)
            {
                throw std::runtime_error("Cannot open data.json");
            }
            saveToAsJSON(file, json);
        }

        std::cout << "All files saved successfully!" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}