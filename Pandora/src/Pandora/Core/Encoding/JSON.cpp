#include "JSON.h"

#include <cctype>

#include "Pandora/Core/IO/FileStream.h"
#include "Pandora/Core/IO/Console.h"
#include "Pandora/Core/Math/Math.h"

// @TODO: currently the JSON parser is not particularly fast or memory-efficient

#define VALIDATE_EXPR(expr, msg) if (!(expr)) {\
    CONSOLE_LOG_DEBUG("{}JSON Error{}: {}\n", ConColor::Red, ConColor::White, (const char*)msg);\
    c->hasError = true;\
    \
    PD_ASSERT_D(false, "JSON error: %s", msg);\
    return;\
}

namespace pd {

// JsonValue

JsonValue::JsonValue(JsonType type) {
    // We want the iterator to be immediately usable
    value = Ref<InternalValue>::Create();
    value->SetType(type);
}

JsonValue::JsonValue(StringView string) {
    value = Ref<InternalValue>::Create();
    Set(string);
}

JsonValue::JsonValue(const char* string)
    : JsonValue(StringView(string)) {
}

JsonValue::JsonValue(f64 number) {
    value = Ref<InternalValue>::Create();
    Set(number);
}

JsonValue::JsonValue(bool boolean) {
    value = Ref<InternalValue>::Create();
    Set(boolean);
}

JsonValue::~JsonValue() {
    Delete();
}

void JsonValue::Delete() {
    value.Reset();
}

void JsonValue::Set(StringView string) {
    SetType(JsonType::String);
    GetString().Set(string);
}

void JsonValue::Set(const char* string) {
    Set(StringView(string));
}

void JsonValue::Set(f64 number) {
    SetType(JsonType::Number);
    GetNumber() = number;
}

void JsonValue::Set(bool boolean) {
    SetType(JsonType::Bool);
    GetBool() = boolean;
}

JsonValue JsonValue::Clone() {
    JsonValue newValue(Type());

    switch (value->type) {
        case JsonType::String:
            newValue.GetString() = GetString();
            break;

        case JsonType::Number:
            newValue.GetNumber() = GetNumber();
            break;

        case JsonType::Object:
            for (int i = 0; i < Count(); i++) {
                newValue.AddField(GetKey(i), GetElement(i));
            }
            break;

        case JsonType::Array:
            for (int i = 0; i < Count(); i++) {
                newValue.AddElement(GetElement(i));
            }
            break;

        case JsonType::Bool:
            newValue.GetBool() = GetBool();
            break;
    }

    return newValue;
}

JsonType JsonValue::Type() const {
    return value->type;
}

void JsonValue::SetType(JsonType type) {
    value->SetType(type);
}

void JsonValue::AddElement(JsonValue value) {
    GetArray().Add(value.Clone());
}

void JsonValue::AddElement(JsonType type) {
    AddElement(JsonValue(type));
}

void JsonValue::AddElement(StringView string) {
    JsonValue tmp;
    tmp = string;
    AddElement(tmp);
}

void JsonValue::AddElement(f64 number) {
    JsonValue tmp;
    tmp = number;
    AddElement(tmp);
}

void JsonValue::AddElement(bool boolean) {
    JsonValue tmp;
    tmp = boolean;
    AddElement(tmp);
}

void JsonValue::AddField(StringView key, JsonValue value) {
    GetObject().Reserve(1);
    GetObject().Last().key.Set(key);
    GetObject().Last().val = value.Clone();
}

String& JsonValue::GetString() const {
    PD_ASSERT_D(Type() == JsonType::String, "JSON type mismatch");
    return value->string;
}

f64& JsonValue::GetNumber() const {
    PD_ASSERT_D(Type() == JsonType::Number, "JSON type mismatch");
    return value->number;
}

JsonObject& JsonValue::GetObject() const {
    PD_ASSERT_D(Type() == JsonType::Object, "JSON type mismatch");
    return value->object;
}

JsonArray& JsonValue::GetArray() const {
    PD_ASSERT_D(Type() == JsonType::Array, "JSON type mismatch");
    return value->array;
}

bool& JsonValue::GetBool() const {
    PD_ASSERT_D(Type() == JsonType::Bool, "JSON type mismatch");
    return value->boolean;
}

Optional<f64> JsonValue::TryGetNumber() {
    Optional<f64> opt;

    if (Type() == JsonType::Number) {
        opt = GetNumber();
    }

    return opt;
}

Optional<bool> JsonValue::TryGetBool() {
    Optional<bool> opt;

    if (Type() == JsonType::Number) {
        opt = GetBool();
    }

    return opt;
}

Optional<String> JsonValue::TryGetString() {
    Optional<String> opt;

    if (Type() == JsonType::String) {
        opt = GetString();
    }

    return opt;
}

Optional<JsonValue> JsonValue::TryGetArray() {
    Optional<JsonValue> opt;

    if (Type() == JsonType::Array) {
        opt = *this;
    }

    return opt;
}

Optional<JsonValue> JsonValue::TryGetObject() {
    Optional<JsonValue> opt;

    if (Type() == JsonType::Object) {
        opt = *this;
    }

    return opt;
}

Optional<f64> JsonValue::TryGetNumber(int index) {
    Optional<f64> opt;

    if (CanEnumerate() && index >= 0 && index < Count()) {
        JsonValue& element = GetElement(index);
        if (element.Type() == JsonType::Number) {
            opt = element.GetNumber();
        }
    }

    return opt;
}

Optional<bool> JsonValue::TryGetBool(int index) {
    Optional<bool> opt;

    if (CanEnumerate() && index >= 0 && index < Count()) {
        JsonValue& element = GetElement(index);
        if (element.Type() == JsonType::Bool) {
            opt = element.GetBool();
        }
    }

    return opt;
}

Optional<String> JsonValue::TryGetString(int index) {
    Optional<String> opt;

    if (CanEnumerate() && index >= 0 && index < Count()) {
        JsonValue& element = GetElement(index);
        if (element.Type() == JsonType::String) {
            opt = element.GetString();
        }
    }

    return opt;
}

Optional<JsonValue> JsonValue::TryGetArray(int index) {
    Optional<JsonValue> opt;

    if (CanEnumerate() && index >= 0 && index < Count()) {
        JsonValue& element = GetElement(index);
        if (element.Type() == JsonType::Array) {
            opt = element;
        }
    }

    return opt;
}

Optional<JsonValue> JsonValue::TryGetObject(int index) {
    Optional<JsonValue> opt;

    if (CanEnumerate() && index >= 0 && index < Count()) {
        JsonValue& element = GetElement(index);
        if (element.Type() == JsonType::Object) {
            opt = element;
        }
    }

    return opt;
}

Optional<f64> JsonValue::TryGetNumber(StringView key) {
    Optional<f64> opt;

    if (Type() == JsonType::Object && HasField(key)) {
        JsonValue& element = GetField(key);
        if (element.Type() == JsonType::Number) {
            opt = element.GetNumber();
        }
    }

    return opt;
}

Optional<bool> JsonValue::TryGetBool(StringView key) {
    Optional<bool> opt;

    if (Type() == JsonType::Object && HasField(key)) {
        JsonValue& element = GetField(key);
        if (element.Type() == JsonType::Bool) {
            opt = element.GetBool();
        }
    }

    return opt;
}

Optional<String> JsonValue::TryGetString(StringView key) {
    Optional<String> opt;

    if (Type() == JsonType::Object && HasField(key)) {
        JsonValue& element = GetField(key);
        if (element.Type() == JsonType::String) {
            opt = element.GetString();
        }
    }

    return opt;
}

Optional<JsonValue> JsonValue::TryGetArray(StringView key) {
    Optional<JsonValue> opt;

    if (Type() == JsonType::Object && HasField(key)) {
        JsonValue& element = GetField(key);
        if (element.Type() == JsonType::Array) {
            opt = element;
        }
    }

    return opt;
}

Optional<JsonValue> JsonValue::TryGetObject(StringView key) {
    Optional<JsonValue> opt;

    if (Type() == JsonType::Object && HasField(key)) {
        JsonValue& element = GetField(key);
        if (element.Type() == JsonType::Object) {
            opt = element;
        }
    }

    return opt;
}

int JsonValue::Count() const {
    if (Type() == JsonType::Array) {
        return GetArray().Count();
    } else {
        return GetObject().Count();
    }
}

bool JsonValue::CanEnumerate() const {
    return Type() == JsonType::Array || Type() == JsonType::Object;
}

String& JsonValue::GetKey(int index) const {
    return GetObject()[index].key;
}

JsonValue& JsonValue::GetElement(int index) const {
    if (Type() == JsonType::Array) {
        return GetArray()[index];
    } else {
        return GetObject()[index].val;
    }
}

bool JsonValue::HasField(StringView key) const {
    return GetFieldIndex(key).HasValue();
}

JsonValue& JsonValue::GetField(StringView key) {
    Optional<int> index = GetFieldIndex(key);

    if (!index) {
        AddField(key, JsonValue());
        return GetObject().Last().val;
    } else {
        return GetObject()[index.Value()].val;
    }
}

JsonValue& JsonValue::operator=(StringView string) {
    Set(string);
    return *this;
}

JsonValue& JsonValue::operator=(const char* text) {
    Set(StringView(text));
    return *this;
}
JsonValue& JsonValue::operator=(double number) {
    Set(number);
    return *this;
}

JsonValue& JsonValue::operator=(bool boolean) {
    Set(boolean);
    return *this;
}

JsonValue& JsonValue::operator[](int index) const {
    return GetElement(index);
}

JsonValue& JsonValue::operator[](StringView key) {
    return GetField(key);
}

Optional<int> JsonValue::GetFieldIndex(StringView key) const {
    PD_ASSERT_D(Type() == JsonType::Object, "JSON type mismatch");

    return value->object.Find([&](Pair<String, JsonValue>& pair) {
        return pair.key == key;
    });
}

bool JsonValue::WriteToFile(StringView path, bool pretty) {
    FileStream file(path, FileMode::Write);

    if (!file.IsOpen()) return false;

    ToStream(file, pretty);

    return true;
}

void JsonValue::ToStream(Stream& stream, bool pretty) {
    if (!stream.CanWrite()) return;

    if (pretty) {
        Log(stream, "{#}", *this);
    } else {
        Log(stream, "{}", *this);
    }
}

JsonValue::InternalValue::InternalValue() {
    SetType(JsonType::Null);
}

JsonValue::InternalValue::~InternalValue() {
    Delete();
}

void JsonValue::InternalValue::Delete() {
    switch (type) {
        case JsonType::String:
            string.Delete();
            break;

        case JsonType::Object:
            object.Delete();
            break;

        case JsonType::Array:
            array.Delete();
            break;
    }
}

void JsonValue::InternalValue::SetType(JsonType type) {
    this->type = type;

    // Call constructor
    switch (type) {
        case JsonType::String:
            new (&string) String();
            break;

        case JsonType::Object:
            new (&object) JsonObject();
            break;

        case JsonType::Array:
            new (&array) JsonArray();
            break;
    }
}

bool JsonValue::Parse(StringView source, bool sourceIsPath, JsonParseSettings settings) {
    if (sourceIsPath) {
        FileStream file(source, FileMode::Read);

        if (!file.IsOpen()) return false;

        return Parse(file, settings);
    }

    MemoryStream stream(source.ToSlice());
    return Parse(stream, settings);
}

bool JsonValue::Parse(Stream& stream, JsonParseSettings settings) {
    ParsingContext c(stream);
    c.stream.SkipBOMIfPresent();
    c.settings = settings;

    if (!c.stream.CanRead()) return false;

    ParseValue(&c, this);
    return !c.hasError;
}

void JsonValue::ParseValue(ParsingContext* c, JsonValue* value) {
    SkipWhitespace(c);
    TryParseComment(c);

    codepoint peek;
    c->stream.PeekCodepoint(&peek);

    switch (peek) {
        case '{':
            value->SetType(JsonType::Object);
            ParseObject(c, &value->GetObject());
            break;

        case '[':
            value->SetType(JsonType::Array);
            ParseArray(c, &value->GetArray());
            break;


        case '"':
            value->SetType(JsonType::String);
            ParseString(c, &value->GetString());
            break;

        default:
            if (peek == '-' || isdigit(peek)) {
                value->SetType(JsonType::Number);
                ParseNumber(c, &value->GetNumber());
            } else {
                // Word must be a keyword
                String word(Allocator::Temporary);

                do {
                    codepoint point;
                    c->stream.ReadCodepoint(&point);

                    // Keywords need to be lower case
                    if (!(point >= 'a' && point <= 'z')) {
                        c->stream.Seek(-CodepointSize(point));
                        break;
                    }

                    word.Append(point);

                } while (c->stream.CanRead());

                bool isTrue = word == "true";
                if (isTrue || word == "false") {
                    value->SetType(JsonType::Bool);
                    value->GetBool() = isTrue;
                } else if (word == "null") {
                    value->SetType(JsonType::Null);
                } else {
                    CONSOLE_LOG_DEBUG("{}JSON Error{}: unknown keyword '{}'\n", ConColor::Red, ConColor::White, word);
                    return;
                }
            }
            break;
    }
}

void JsonValue::ParseString(ParsingContext* c, String* value) {
    c->stream.Seek(1); // Consume starting quote

    u64 position = c->stream.Position();

    codepoint point;
    while (c->stream.CanRead()) {
        VALIDATE_EXPR(c->stream.ReadCodepoint(&point) != 0, "unexpected end-of-stream");

        if (point == '\\') {
            codepoint escaped;

            VALIDATE_EXPR(c->stream.ReadCodepoint(&escaped) != 0, "unexpected end-of-stream");

            switch (escaped) {
                case '"':
                    value->Append('"');
                    break;

                case '\\':
                    value->Append("\\");
                    break;

                case '/':
                    value->Append("/");
                    break;

                case 'b':
                    value->Append("\b");
                    break;

                case 'f':
                    value->Append("\f");
                    break;

                case 'n':
                    value->Append("\n");
                    break;

                case 'r':
                    value->Append("\r");
                    break;

                case 't':
                    value->Append("\t");
                    break;

                case 'u':
                    // Hex digits must be [ 0..9 A..F a..f ] which are all 1 byte long
                    char hex[5];
                    hex[4] = '\0';

                    // Read the 4 characters
                    codepoint tmp;
                    VALIDATE_EXPR(c->stream.ReadCodepoint(&tmp) == 1, "illegal hex character");
                    hex[0] = (char)tmp;
                    VALIDATE_EXPR(c->stream.ReadCodepoint(&tmp) == 1, "illegal hex character");
                    hex[1] = (char)tmp;
                    VALIDATE_EXPR(c->stream.ReadCodepoint(&tmp) == 1, "illegal hex character");
                    hex[2] = (char)tmp;
                    VALIDATE_EXPR(c->stream.ReadCodepoint(&tmp) == 1, "illegal hex character");
                    hex[3] = (char)tmp;

                    // Validate the ascii characters
                    for (int i = 0; i < 4; i++) {
                        VALIDATE_EXPR((hex[i] >= 'A' && hex[i] <= 'F') ||
                                      (hex[i] >= 'a' && hex[i] <= 'f') ||
                                      isdigit(hex[i]), "illegal hex character");
                    }

                    // Finally, append the codepoint
                    codepoint parsed;
                    sscanf(hex, "%x", &parsed);

                    value->Append(parsed);
                    break;
            }

        } else if (point == '"') {
            break;
        } else {
            value->Append(point);
        }
    }
}

void JsonValue::ParseNumber(ParsingContext* c, f64* value) {
    bool negative = c->stream.SkipIfEqual("-");

    int radixCount = 0;
    bool hasExponent = false;
    codepoint point;

    String number(Allocator::Temporary);
    do {
        if (c->stream.ReadCodepoint(&point) == 0) break;

        if (point == 'e' || point == 'E') {
            hasExponent = true;
            break;
        }

        if (point == '.') {
            radixCount += 1;
        } else if (!isdigit(point)) {
            // We're reading things we're not supposed to, abort
            c->stream.Seek(-CodepointSize(point));
            break;
        }

        number.Append(point);

    } while (isdigit(point) || (point == '.' && radixCount == 1));

    f64 base;
    sscanf(number.CStr(), "%lf", &base);

    number.Set("");

    if (hasExponent) {
        VALIDATE_EXPR(c->stream.ReadCodepoint(&point) != 0, "unexpected end-of-stream");
        VALIDATE_EXPR(point == '+' || point == '-' || isdigit(point), "illegal base sign");

        if (isdigit(point)) {
            c->stream.Seek(-1);
        }

        bool basePositive = true;
        if (point == '-') {
            basePositive = false;
        }

        radixCount = 0;
        do {
            if (c->stream.ReadCodepoint(&point) == 0) break;

            if (c->settings.allowExponentDecimals && point == '.') {
                radixCount += 1;
            } else if (point == '.') {
                break;
            } else if (!isdigit(point)) {
                c->stream.Seek(-CodepointSize(point));
                break;
            }

            number.Append(point);

        } while (isdigit(point) || (point == '.' && radixCount == 1));

        f64 exp;
        sscanf(number.CStr(), "%lf", &exp);

        if (!basePositive) {
            exp *= -1.0;
        }

        base *= Pow(10.0, exp);
    }

    *value = base * ((negative) ? -1.0 : 1.0);
}

void JsonValue::ParseObject(ParsingContext* c, JsonObject* value) {
    // Consume starting brace
    c->stream.Seek(1);

    SkipWhitespace(c);

    const auto parseField = [&]() {
        // Parse field
        codepoint point;
        value->Reserve(1);

        SkipWhitespace(c);
        TryParseComment(c);

        VALIDATE_EXPR(c->stream.PeekCodepoint(&point) != 0, "unexpected end-of-stream");
        VALIDATE_EXPR(point == '"', "object field key must be a string");

        ParseString(c, &value->Last().key);

        SkipWhitespace(c);
        VALIDATE_EXPR(c->stream.ReadCodepoint(&point) != 0, "unexpected end-of-stream");
        VALIDATE_EXPR(point == ':', "illegal token after field key");

        ParseValue(c, &value->Last().val);
    };

    codepoint point;
    VALIDATE_EXPR(c->stream.PeekCodepoint(&point) != 0, "unexpected end-of-stream");
    if (point != '}') {
        parseField();
        SkipWhitespace(c);

        VALIDATE_EXPR(c->stream.ReadCodepoint(&point) != 0, "unexpected end-of-stream");

        if (point != ',') {
            VALIDATE_EXPR(point == '}', "illegal token after field");
            return;
        }

        while (c->stream.CanRead()) {
            parseField();

            SkipWhitespace(c);
            VALIDATE_EXPR(c->stream.ReadCodepoint(&point) != 0, "unexpected end-of-stream");
            if (point == '}') break;
            VALIDATE_EXPR(point == ',', "illegal token after field");
        }

    } else {
        // Consume closing brace
        SkipWhitespace(c);
        c->stream.Seek(1);
    }
}

void JsonValue::ParseArray(ParsingContext* c, JsonArray* value) {
    // Consume starting bracket
    c->stream.Seek(1);

    SkipWhitespace(c);

    codepoint point;
    VALIDATE_EXPR(c->stream.PeekCodepoint(&point) != 0, "unexpected end-of-stream");
    if (point != ']') {
        while (c->stream.CanRead()) {
            value->Reserve(1);
            ParseValue(c, &value->Last());

            SkipWhitespace(c);
            VALIDATE_EXPR(c->stream.ReadCodepoint(&point) != 0, "unexpected end-of-stream");

            if (c->settings.allowComments && point == '/') {
                c->stream.Seek(-1);
                TryParseComment(c);
                VALIDATE_EXPR(c->stream.ReadCodepoint(&point) != 0, "unexpected end-of-stream");
            }

            if (point == ']') break;

            VALIDATE_EXPR(point == ',', "illegal token in array");
        }
    } else {
        // Consume closing bracket
        c->stream.Seek(1);
    }
}

void JsonValue::TryParseComment(ParsingContext* c) {
    if (c->settings.allowComments) {
        codepoint peek;
        c->stream.PeekCodepoint(&peek);

        while (peek == '/') {
            // Check if next character is also a '/'
            c->stream.ReadCodepoint(&peek);
            c->stream.PeekCodepoint(&peek);
            if (peek == '/') {
                String dummy(Allocator::Temporary);
                c->stream.ReadLine(dummy);
                SkipWhitespace(c);
                c->stream.PeekCodepoint(&peek);
            } else {
                // Nope, seek backwards and stop
                c->stream.Seek(-1);
                break;
            }
        }
    }
}

void JsonValue::SkipWhitespace(ParsingContext* c) {
    codepoint point;
    while (c->stream.CanRead()) {
        if (c->stream.ReadCodepoint(&point) == 0) return;

        if (point != ' ' &&
            point != '\n' &&
            point != '\r' &&
            point != '\t') {
            c->stream.Seek(-CodepointSize(point));
            return;
        }
    }
}

}

#undef VALIDATE_EXPR
