#pragma once

#include "Pandora/Core/Data/StringView.h"
#include "Pandora/Core/Data/String.h"
#include "Pandora/Core/Data/Array.h"
#include "Pandora/Core/Data/Pair.h"
#include "Pandora/Core/Data/Reference.h"
#include "Pandora/Core/Data/Optional.h"
#include "Pandora/Core/IO/Stream.h"
#include "Pandora/Core/IO/Console.h"

namespace pd {

enum class JsonType {
    Null,
    String,
    Number,
    Object,
    Array,
    Bool
};

class JsonValue;
typedef Array<Pair<String, JsonValue>> JsonObject;
typedef Array<JsonValue> JsonArray;

/**
 * \brief These are non-standard options parsing options.
 */
struct JsonParseSettings {
    /**
     * \brief Allows floating point numbers in the exponent, e.g. 10e2.4
     */
    bool allowExponentDecimals = false;

    /**
     * \brief Allows single-line comments with //
     */
    bool allowComments = false;
};

class JsonValue {
public:

    /**
     * \brief Creates a new value with a specified type.
     * 
     * \param type The type.
     */
    JsonValue(JsonType type = JsonType::Null);

    JsonValue(StringView string);
    JsonValue(const char* string);
    JsonValue(f64 number);
    JsonValue(bool boolean);

    ~JsonValue();

    /**
     * \brief Resets the internal reference.
     */
    void Delete();

    /**
     * \brief Sets the type and value to a string.
     * 
     * \param string The string value.
     */
    void Set(StringView string);

    /**
     * \brief Sets the type and value to a string.
     * 
     * \param string The string value.
     */
    void Set(const char* string);

    /**
     * \brief Sets the type and value to a number.
     * 
     * \param number The number value.
     */
    void Set(f64 number);

    /**
     * \brief Sets the type and value to a boolean.
     * 
     * \param boolean The boolean value.
     */
    void Set(bool boolean);

    /**
     * \brief Parses either a JSON file or direct source.
     * 
     * \param source Either a path to a JSON file or the JSON source itself.
     * \param sourceIsPath True if `source` is a path, false if it is source.
     * \param settings Any custom settings for the JSON parser.
     * \return Whether or not it parsed successfully.
     */
    bool Parse(StringView source, bool sourceIsPath = true, JsonParseSettings settings = JsonParseSettings());

    /**
     * \brief Parses JSON from an input stream.
     * 
     * \param stream The input stream.
     * \param settings Any custom settings for the JSON parser.
     * \return Whether or not it parsed successfully.
     */
    bool Parse(Stream& stream, JsonParseSettings settings = JsonParseSettings());

    /**
     * \brief Creates a deep copy of this value.
     * 
     * \return The cloned copy.
     */
    JsonValue Clone();

    /**
     * \return The type of the JSON value.
     */
    JsonType Type() const;

    /**
     * \brief Sets the type of the JSON value. Destructs any necessary memory.
     * 
     * \param type The new type.
     */
    void SetType(JsonType type);

    /**
     * \brief Adds a value to the array.
     * Only works if this value is a `JsonType::Array`.
     * 
     * \param value The JSON value.
     */
    void AddElement(JsonValue value);

    /**
     * \brief Adds a value of the specified type to the array.
     * Only works if this value is a `JsonType::Array`.
     * 
     * \param type The type.
     */
    void AddElement(JsonType type = JsonType::Null);

    /**
     * \brief Adds a string value to the array.
     * Only works if this value is a `JsonType::Array`.
     * 
     * \param string The string value.
     */
    void AddElement(StringView string);

    /**
     * \brief Adds a number value to the array.
     * Only works if this value is a `JsonType::Array`.
     * 
     * \param number The number value.
     */
    void AddElement(f64 number);

    /**
     * \brief Adds a boolean value to the array.
     * Only works if this value is a `JsonType::Array`.
     * 
     * \param boolean The boolean value.
     */
    void AddElement(bool boolean);

    /**
     * \brief Adds a field to the object.
     * Only works if this value is a `JsonType::Object`.
     * 
     * \param key The field key.
     * \param value The field value.
     */
    void AddField(StringView key, JsonValue value);

    /**
     * \brief Gets the string value.
     * Only works if the value is a `JsonType::String`.
     * 
     * \return The string value.
     */
    String& GetString() const;

    /**
     * \brief Gets the number value.
     * Only works if the value is a `JsonType::Number`.
     * 
     * \return The number value.
     */
    f64& GetNumber() const;

    /**
     * \brief Gets the object value.
     * Only works if the value is a `JsonType::Object`.
     * 
     * \return The object value.
     */
    JsonObject& GetObject() const;

    /**
     * \brief Gets the array value.
     * Only works if the type is a `JsonType::Array`.
     * 
     * \return The array value.
     */
    JsonArray& GetArray() const;
 
    /**
     * \brief Gets the boolean value.
     * Only works if the value is a `JsonType::Boolean`.
     * 
     * \return The boolean value.
     */
    bool& GetBool() const;

    /**
     * \brief Attempts to get the number value.
     * 
     * \return The output value.
     */
    Optional<f64> TryGetNumber();

    /**
     * \brief Attempts to get the boolean value.
     * 
     * \return The output value.
     */
    Optional<bool> TryGetBool();

    /**
     * \brief Attempts to get the string value.
     * 
     * \return The output value.
     */
    Optional<String> TryGetString();

    /**
     * \brief Attempts to get the array value.
     * 
     * \return The output value.
     */
    Optional<JsonValue> TryGetArray();

    /**
     * \brief Attempts to get the object value.
     * 
     * \return The output value.
     */
    Optional<JsonValue> TryGetObject();

    /**
     * \brief Attempts to get the number value from an array or object.
     * Only works if the value is a `JsonType::Object` or `JsonType::Array`.
     * 
     * \param index The array/field index.
     * \return The output value.
     */
    Optional<f64> TryGetNumber(int index);

    /**
     * \brief Attempts to get the boolean value from an array or object.
     * Only works if the value is a `JsonType::Object` or `JsonType::Array`.
     * 
     * \param index The array/field index.
     * \param target The output value.
     * \return Whether or not it succeeded.
     */
    Optional<bool> TryGetBool(int index);

    /**
     * \brief Attempts to get the string value from an array or object.
     * Only works if the value is a `JsonType::Object` or `JsonType::Array`.
     * 
     * \param index The array/field index.
     * \return The output value.
     */
    Optional<String> TryGetString(int index);

    /**
     * \brief Attempts to get the array value from an array or object.
     * Only works if the value is a `JsonType::Object` or `JsonType::Array`.
     * 
     * \param index The array/field index.
     * \return The output value.
     */
    Optional<JsonValue> TryGetArray(int index);

    /**
     * \brief Attempts to get the object value from an array or object.
     * Only works if the value is a `JsonType::Object` or `JsonType::Array`.
     * 
     * \param index The array/field index.
     * \return The output value.
     */
    Optional<JsonValue> TryGetObject(int index);

    /**
     * \brief Attempts to get the number value from a field.
     * Only works if the value is a `JsonType::Object`.
     * 
     * \param key The field key.
     * \return The output value.
     */
    Optional<f64> TryGetNumber(StringView key);

    /**
     * \brief Attempts to get the boolean value from a field.
     * Only works if the value is a `JsonType::Object`.
     * 
     * \param key The field key.
     * \return The output value.
     */
    Optional<bool> TryGetBool(StringView key);

    /**
     * \brief Attempts to get the string value from a field.
     * Only works if the value is a `JsonType::Object`.
     * 
     * \param key The field key.
     * \return The output value.
     */
    Optional<String> TryGetString(StringView key);

    /**
     * \brief Attempts to get the array value from a field.
     * Only works if the value is a `JsonType::Object`.
     * 
     * \param key The field key.
     * \return The output value.
     */
    Optional<JsonValue> TryGetArray(StringView key);

    /**
     * \brief Attempts to get the object value from a field.
     * Only works if the value is a `JsonType::Object`.
     * 
     * \param key The field key.
     * \return The output value.
     */
    Optional<JsonValue> TryGetObject(StringView key);

    /**
     * \return How many elements/fields the array/object has.
     */
    int Count() const;

    /**
     * \return An enumerable type is either a `JsonType::Array` or a `JsonType::Object`.
     * An enumerable value means it supports `Count()` and `GetElement()`.
     */
    bool CanEnumerate() const;

    /**
     * \brief Gets the key of the element at the index.
     * Only works if the value is a `JsonType::Object`.
     * 
     * \param index The index of the field.
     * \return The field key.
     */
    String& GetKey(int index) const;

    /**
     * \brief Gets the value of the element/field of the array/object.
     * Only works if the value is a `JsonType::Array` or `JsonType::Object`.
     * 
     * \param index The index of the element/field.
     * \return The value at the specified index.
     */
    JsonValue& GetElement(int index) const;

    /**
     * \brief Checks if the field exists.
     * Only works if the value is a `JsonType::Object`.
     * 
     * \param key The field key.
     * \return Whether or not the field exists.
     */
    bool HasField(StringView key) const;

    /**
     * \brief Gets the field with the specified key.
     * Creates the field if it doesn't exist.
     * Only works if the value is a `JsonType::Object`.
     * 
     * \param key The field key.
     * \return The field value.
     */
    JsonValue& GetField(StringView key);

    /**
     * \brief Finds the index of a field.
     * 
     * \param key The field key.
     * \return The index of the field, if found.
     */
    Optional<int> GetFieldIndex(StringView key) const;

    /**
     * \brief Writes the JSON value to a file.
     * 
     * \param path The output path.
     * \param pretty Whether or not to write prettified JSON.
     * \return Whether or not it wrote it successfully.
     */
    bool WriteToFile(StringView path, bool pretty = true);

    /**
     * \brief Writes the JSON value to the output stream.
     * 
     * \param stream The output stream.
     * \param pretty Whether or not to pretty-print it.
     */
    void ToStream(Stream& stream, bool pretty = true);

    /**
     * \brief Calls `Set()`.
     * 
     * \param string The string value.
     * \return This value.
     */
    JsonValue& operator=(StringView string);

    /**
     * \brief Calls `Set()`.
     * 
     * \param text The string value.
     * \return This value.
     */
    JsonValue& operator=(const char* text);

    /**
     * \brief Calls `Set()`.
     * 
     * \param number The number value.
     * \return This value.
     */
    JsonValue& operator=(f64 number);

    /**
     * \brief Calls `Set()`.
     * 
     * \param boolean The boolean value.
     * \return This value.
     */
    JsonValue& operator=(bool boolean);

    /**
     * \brief Calls `GetElement()`.
     * 
     * \param index The element/field index.
     * \return This element/field value.
     */
    JsonValue& operator[](int index) const;

    /**
     * \brief Calls `GetField()`.
     * 
     * \param key The field key.
     * \return The field value.
     */
    JsonValue& operator[](StringView key);

private:
    struct InternalValue {
        InternalValue();
        ~InternalValue();

        /**
         * \brief Frees the appropiate union members. Gets called on destruction.
         */
        void Delete();

        /**
         * \brief Sets the type and initializes the union.
         * 
         * \param type The type.
         */
        void SetType(JsonType type);

        JsonType type = JsonType::Null;
        union {
            String string;
            f64 number;
            JsonObject object;
            JsonArray array;
            bool boolean;
        };
    };

    struct ParsingContext {
        ParsingContext(Stream& stream) : stream(stream) {}

        Stream& stream;
        bool hasError = false;

        JsonParseSettings settings;
    };

    void ParseValue(ParsingContext* c, JsonValue* value);

    void ParseString(ParsingContext* c, String* value);
    void ParseNumber(ParsingContext* c, f64* value);
    void ParseObject(ParsingContext* c, JsonObject* value);
    void ParseArray(ParsingContext* c, JsonArray* value);
    void TryParseComment(ParsingContext* c);

    void SkipWhitespace(ParsingContext* c);

    Ref<InternalValue> value;
};

template<>
inline void PrintType(JsonType& type, FormatInfo& info) {
    switch (type) {
        case JsonType::Null:
            PrintfToStream(info.output, "null");
            break;

        case JsonType::String:
            PrintfToStream(info.output, "string");
            break;

        case JsonType::Number:
            PrintfToStream(info.output, "number");
            break;

        case JsonType::Object:
            PrintfToStream(info.output, "object");
            break;

        case JsonType::Array:
            PrintfToStream(info.output, "array");
            break;

        case JsonType::Bool:
            PrintfToStream(info.output, "bool");
            break;
    }
}

template<>
inline void PrintType(JsonValue& type, FormatInfo& info) {
    // Not sure if there's a sane way to make this specifiable without doing custom parsing of the raw
    const int INDENT_STEP = 2;
    const char INDENT_CHAR = ' ';

    bool wasPretty = info.pretty;

    // We abuse the precision field as our indentation
    int oldPrecision = info.precision;
    bool wasSpecified = info.precisionSpecified;

    info.precisionSpecified = false;

    auto printIndent = [&]() {
        if (wasPretty) {
            char ident = ' ';
            info.precisionSpecified = true;
            PrintType(ident, info);
            info.precisionSpecified = false;
        }
    };

    auto setColor = [&](ConColor color) {
        if (&info.output == &console) {
            console.SetColor(color);
        }
    };

    switch (type.Type()) {
        case JsonType::Null:
            setColor(ConColor::Blue);
            PrintfToStream(info.output, "null");
            setColor(ConColor::White);
            break;

        case JsonType::String:
            setColor(ConColor::Red);
            info.pretty = true;
            PrintType(type.GetString(), info);
            info.pretty = wasPretty;
            setColor(ConColor::White);
            break;

        case JsonType::Number:
            setColor(ConColor::Yellow);
            PrintType(type.GetNumber(), info);
            setColor(ConColor::White);
            break;

        case JsonType::Object:
            PrintfToStream(info.output, "{%s", (wasPretty) ? "\n" : "");

            info.precision += INDENT_STEP;

            for (int i = 0; i < type.Count(); i++) {
                printIndent();

                info.pretty = true;
                setColor(ConColor::Cyan);
                PrintType(type.GetKey(i), info);
                setColor(ConColor::White);
                PrintfToStream(info.output, ":%s", (wasPretty) ? " " : "");

                info.pretty = wasPretty;
                PrintType(type.GetElement(i), info);

                if (i + 1 < type.Count()) {
                    PrintfToStream(info.output, ",%s", (wasPretty) ? "\n" : "");
                }
            }

            if (wasPretty) {
                PrintfToStream(info.output, "\n");
            }

            info.precision -= INDENT_STEP;

            printIndent();
            PrintfToStream(info.output, "}");
            break;

        case JsonType::Array:
            PrintfToStream(info.output, "[%s", (wasPretty) ? "\n" : "");

            info.precision += INDENT_STEP;

            for (int i = 0; i < type.Count(); i++) {
                printIndent();
                PrintType(type.GetElement(i), info);

                if (i + 1 < type.Count()) {
                    PrintfToStream(info.output, ",%s", (wasPretty) ? "\n" : "");
                }
            }

            if (wasPretty) {
                PrintfToStream(info.output, "\n");
            }

            info.precision -= INDENT_STEP;

            printIndent();
            PrintfToStream(info.output, "]");
            break;

        case JsonType::Bool:
            setColor(ConColor::Blue);
            PrintType(type.GetBool(), info);
            setColor(ConColor::White);
            break;
    }

    info.pretty = wasPretty;
    info.precisionSpecified = wasSpecified;
    info.precision = oldPrecision;
}

}
