#pragma once

#include "Pandora/Core/Data/StringView.h"
#include "Pandora/Core/Data/String.h"
#include "Pandora/Core/Data/Array.h"
#include "Pandora/Core/Data/Pair.h"
#include "Pandora/Core/Data/Reference.h"

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

struct JsonParseSettings {
    // Allow floating-point numbers in the number exponent e.g. 10e2.4
    bool allowExponentDecimals = false;
    // Allow single-line comments with "//"
    bool allowComments = false;
};

class JsonValue {
public:
    /// <summary>
    /// Creates a new <c>JsonValue</c> with the specified type.
    /// </summary>
    /// <param name="type">The type.</param>
    JsonValue(JsonType type = JsonType::Null);

    JsonValue(StringView string);
    JsonValue(const uchar* string);
    JsonValue(f64 number);
    JsonValue(bool boolean);

    /// <summary>
    /// Sets the type and value to a string.
    /// </summary>
    /// <param name="string">The string value.</param>
    void Set(StringView string);

    /// <summary>
    /// Sets the type and value to a string.
    /// </summary>
    /// <param name="string">The string value.</param>
    void Set(const uchar* string);

    /// <summary>
    /// Sets the type and value to a number.
    /// </summary>
    /// <param name="string">The number value.</param>
    void Set(f64 number);

    /// <summary>
    /// Sets the type and value to a bool.
    /// </summary>
    /// <param name="string">The bool value.</param>
    void Set(bool boolean);

    /// <summary>
    /// Parses JSON from either a file path or direct source.
    /// </summary>
    /// <param name="source">Either the JSON source or path.</param>
    /// <param name="sourceIsPath">Whether or not <c>source</c> is a path to a file.</param>
    bool Parse(StringView source, bool sourceIsPath = true, JsonParseSettings settings = JsonParseSettings());

    /// <summary>
    /// Parses JSON from a stream.
    /// </summary>
    /// <param name="stream">The stream.</param>
    bool Parse(Stream* stream, JsonParseSettings settings = JsonParseSettings());

    /// <summary>
    /// Creates a deep copy of this value.
    /// </summary>
    /// <returns>The clone.</returns>
    JsonValue Clone();

    /// <summary>
    /// Types this instance.
    /// </summary>
    /// <returns></returns>
    JsonType Type() const;

    /// <summary>
    /// Sets the type of the JSON value. Destructs any necessary memory.
    /// </summary>
    /// <param name="type">The type.</param>
    void SetType(JsonType type);

    /// <summary>
    /// Adds a value to our array.
    /// </summary>
    /// <param name="value">The value. This gets cloned.</param>
    void AddElement(JsonValue value);

    /// <summary>
    /// Adds a value to this array with the specified type.
    /// </summary>
    void AddElement(JsonType type = JsonType::Null);

    /// <summary>
    /// Adds a value to our array.
    /// </summary>
    /// <param name="string">The string.</param>
    void AddElement(StringView string);

    /// <summary>
    /// Adds a value to our array.
    /// </summary>
    /// <param name="string">The number.</param>
    void AddElement(f64 number);

    /// <summary>
    /// Adds a value to our array.
    /// </summary>
    /// <param name="string">The bool.</param>
    void AddElement(bool boolean);

    /// <summary>
    /// Adds a field to our object.
    /// </summary>
    /// <param name="key">The key.</param>
    /// <param name="value">The value. This gets cloned.</param>
    void AddField(StringView key, JsonValue value);

    /// <summary>
    /// Gets the string type. Type must be a <c>JsonType::String</c>.
    /// </summary>
    /// <returns>The string type.</returns>
    String& GetString() const;

    /// <summary>
    /// Gets the number type. Type must be a <c>JsonType::Number</c>.
    /// </summary>
    /// <returns>The number type.</returns>
    f64& GetNumber() const;

    /// <summary>
    /// Gets the object type. Type must be a <c>JsonType::Object</c>.
    /// </summary>
    /// <returns>The object type.</returns>
    JsonObject& GetObject() const;

    /// <summary>
    /// Gets the array type. Type must be a <c>JsonType::Array</c>.
    /// </summary>
    /// <returns>The array type.</returns>
    JsonArray& GetArray() const;

    /// <summary>
    /// Gets the bool type. Type must be a <c>JsonType::Bool</c>.
    /// </summary>
    /// <returns>The bool type.</returns>
    bool& GetBool() const;
    
    /// <summary>
    /// Attempts to get this value as a number.
    /// </summary>
    /// <param name="target">The target.</param>
    /// <returns>Whether it succeeded or not.</returns>
    bool TryGetNumber(f64& target);

    /// <summary>
    /// Attempts to get this value as a boolean.
    /// </summary>
    /// <param name="target">The target.</param>
    /// <returns>Whether it succeeded or not.</returns>
    bool TryGetBool(bool& target);

    /// <summary>
    /// Attempts to get this value as a string.
    /// </summary>
    /// <param name="target">The target.</param>
    /// <returns>Whether it succeeded or not.</returns>
    bool TryGetString(String& target);

    /// <summary>
    /// Attempts to get this value as an array.
    /// </summary>
    /// <param name="target">The target.</param>
    /// <returns>Whether it succeeded or not.</returns>
    bool TryGetArray(JsonValue& target);

    /// <summary>
    /// Attempts to get this value as an object.
    /// </summary>
    /// <param name="target">The target.</param>
    /// <returns>Whether it succeeded or not.</returns>
    bool TryGetObject(JsonValue& target);

    /// <summary>
    /// Attempts to get the value at the index of the array/object as a number.
    /// </summary>
    /// <param name="target">The target.</param>
    /// <returns>Whether it succeeded or not.</returns>
    bool TryGetNumber(int index, f64& target);

    /// <summary>
    /// Attempts to get the value at the index of the array/object as a boolean.
    /// </summary>
    /// <param name="target">The target.</param>
    /// <returns>Whether it succeeded or not.</returns>
    bool TryGetBool(int index, bool& target);

    /// <summary>
    /// Attempts to get the value at the index of the array/object as a string.
    /// </summary>
    /// <param name="target">The target.</param>
    /// <returns>Whether it succeeded or not.</returns>
    bool TryGetString(int index, String& target);

    /// <summary>
    /// Attempts to get the value at the index of the array/object as an array.
    /// </summary>
    /// <param name="target">The target.</param>
    /// <returns>Whether it succeeded or not.</returns>
    bool TryGetArray(int index, JsonValue& target);

    /// <summary>
    /// Attempts to get the value at the index of the array/object as an object.
    /// </summary>
    /// <param name="target">The target.</param>
    /// <returns>Whether it succeeded or not.</returns>
    bool TryGetObject(int index, JsonValue& target);

    /// <summary>
    /// Attempts to get the value of the field of the object as a number.
    /// </summary>
    /// <param name="target">The target.</param>
    /// <returns>Whether it succeeded or not.</returns>
    bool TryGetNumber(StringView key, f64& target);

    /// <summary>
    /// Attempts to get the value of the field of the object as a boolean.
    /// </summary>
    /// <param name="target">The target.</param>
    /// <returns>Whether it succeeded or not.</returns>
    bool TryGetBool(StringView key, bool& target);

    /// <summary>
    /// Attempts to get the value of the field of the object as a string.
    /// </summary>
    /// <param name="target">The target.</param>
    /// <returns>Whether it succeeded or not.</returns>
    bool TryGetString(StringView key, String& target);

    /// <summary>
    /// Attempts to get the value of the field of the object as an array.
    /// </summary>
    /// <param name="target">The target.</param>
    /// <returns>Whether it succeeded or not.</returns>
    bool TryGetArray(StringView key, JsonValue& target);

    /// <summary>
    /// Attempts to get the value of the field of the object as an object.
    /// </summary>
    /// <param name="target">The target.</param>
    /// <returns>Whether it succeeded or not.</returns>
    bool TryGetObject(StringView key, JsonValue& target);

    /// <summary>
    /// Returns how many elements/fields the array/object has.
    /// </summary>
    /// <returns>How many elements/fields the array/object has.</returns>
    int Count() const;

    /// <summary>
    /// Returns <c>true</c> if you can call <c>Count()</c> and <c>GetElement()</c> on this type.
    /// This is <c>true</c> if the type is <c>JsonType::Array</c> or <c>JsonType::Object</c>.
    /// </summary>
    /// <returns>Whether or not the type is enumerable.</returns>
    bool CanEnumerate() const;

    /// <summary>
    /// Gets the key of the element at the index.
    /// </summary>
    /// <param name="index">The index.</param>
    /// <returns>The key.</returns>
    String& GetKey(int index) const;

    /// <summary>
    /// Returns the element/field at the index of the array/object.
    /// </summary>
    /// <param name="index">The index.</param>
    /// <returns></returns>
    JsonValue& GetElement(int index) const;

    /// <summary>
    /// Checks if the field exists.
    /// </summary>
    /// <param name="key">The key.</param>
    /// <returns>Whether or not it exists.</returns>
    bool HasField(StringView key) const;

    /// <summary>
    /// Gets the field with the specified key.
    /// Creates the field if it doesn't exist.
    /// </summary>
    /// <param name="key">The key.</param>
    /// <returns>The type of the field.</returns>
    JsonValue& GetField(StringView key);

    /// <summary>
    /// Finds the index of a field. -1 if the field was not found.
    /// </summary>
    /// <param name="key">The key.</param>
    /// <returns>The index of the field.</returns>
    int GetFieldIndex(StringView key) const;

    /// <summary>
    /// Writes this value to a file.
    /// </summary>
    /// <param name="path">The path.</param>
    /// <param name="pretty">Whether or not to pretty-print it.</param>
    /// <returns>Whether or not it was successful.</returns>
    bool WriteToFile(StringView path, bool pretty = true);

    /// <summary>
    /// Writes this value to a stream.
    /// </summary>
    /// <param name="stream">The stream.</param>
    /// <param name="pretty">Whether or not to pretty-print it.</param>
    void ToStream(Stream* stream, bool pretty = true);

    JsonValue& operator=(StringView string);
    JsonValue& operator=(const uchar* text);
    JsonValue& operator=(double number);
    JsonValue& operator=(bool boolean);

    JsonValue& operator[](int index) const;
    JsonValue& operator[](StringView key);

private:
    struct InternalValue {
        InternalValue();
        ~InternalValue();

        /// <summary>
        /// Frees the memory of the union. This is called on destruction.
        /// </summary>
        void Delete();

        /// <summary>
        /// Sets the type and initializes the union.
        /// </summary>
        /// <param name="type">The type.</param>
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
        Stream* stream = nullptr;
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
inline void PrintType(JsonType* type, FormatInfo* info) {
    switch (*type) {
        case JsonType::Null:
            PrintfToStream(info->output, "null");
            break;

        case JsonType::String:
            PrintfToStream(info->output, "string");
            break;

        case JsonType::Number:
            PrintfToStream(info->output, "number");
            break;

        case JsonType::Object:
            PrintfToStream(info->output, "object");
            break;

        case JsonType::Array:
            PrintfToStream(info->output, "array");
            break;

        case JsonType::Bool:
            PrintfToStream(info->output, "bool");
            break;
    }
}

template<>
inline void PrintType(JsonValue* type, FormatInfo* info) {
    // Not sure if there's a sane way to make this specifiable without doing custom parsing of the raw
    const int INDENT_STEP = 2;
    const char INDENT_CHAR = ' ';

    bool wasPretty = info->pretty;

    // We abuse the precision field as our indentation
    int oldPrecision = info->precision;
    bool wasSpecified = info->precisionSpecified;

    info->precisionSpecified = false;

    auto printIndent = [&]() {
        if (wasPretty) {
            char ident = ' ';
            info->precisionSpecified = true;
            PrintType(&ident, info);
            info->precisionSpecified = false;
        }
    };

    auto setColor = [&](ConColor color) {
        if (info->output == &console) {
            console.SetColor(color);
        }
    };

    switch (type->Type()) {
        case JsonType::Null:
            setColor(ConColor::Blue);
            PrintfToStream(info->output, "null");
            setColor(ConColor::White);
            break;

        case JsonType::String:
            setColor(ConColor::Red);
            info->pretty = true;
            PrintType(&type->GetString(), info);
            info->pretty = wasPretty;
            setColor(ConColor::White);
            break;

        case JsonType::Number:
            setColor(ConColor::Yellow);
            PrintType(&type->GetNumber(), info);
            setColor(ConColor::White);
            break;

        case JsonType::Object:
            PrintfToStream(info->output, "{%s", (wasPretty) ? "\n" : "");

            info->precision += INDENT_STEP;

            for (int i = 0; i < type->Count(); i++) {
                printIndent();

                info->pretty = true;
                setColor(ConColor::Cyan);
                PrintType(&type->GetKey(i), info);
                setColor(ConColor::White);
                PrintfToStream(info->output, ":%s", (wasPretty) ? " " : "");

                info->pretty = wasPretty;
                PrintType(&type->GetElement(i), info);

                if (i + 1 < type->Count()) {
                    PrintfToStream(info->output, ",%s", (wasPretty) ? "\n" : "");
                }
            }

            if (wasPretty) {
                PrintfToStream(info->output, "\n");
            }

            info->precision -= INDENT_STEP;

            printIndent();
            PrintfToStream(info->output, "}");
            break;

        case JsonType::Array:
            PrintfToStream(info->output, "[%s", (wasPretty) ? "\n" : "");

            info->precision += INDENT_STEP;

            for (int i = 0; i < type->Count(); i++) {
                printIndent();
                PrintType(&type->GetElement(i), info);

                if (i + 1 < type->Count()) {
                    PrintfToStream(info->output, ",%s", (wasPretty) ? "\n" : "");
                }
            }

            if (wasPretty) {
                PrintfToStream(info->output, "\n");
            }

            info->precision -= INDENT_STEP;

            printIndent();
            PrintfToStream(info->output, "]");
            break;

        case JsonType::Bool:
            setColor(ConColor::Blue);
            PrintType(&type->GetBool(), info);
            setColor(ConColor::White);
            break;
    }

    info->pretty = wasPretty;
    info->precisionSpecified = wasSpecified;
    info->precision = oldPrecision;
}

}
