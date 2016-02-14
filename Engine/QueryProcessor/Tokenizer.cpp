#include "Tokenizer.h"

namespace HushDB
{
    vector<SqlToken> Tokenizer::Parse(const String& query)
    {
        vector<SqlToken> tokens;

        const Char* reading = query.c_str();
        const Char* begin = nullptr;
        const Char* rowBegin = reading;

        int rowNumber = 1;
        State state = State::Begin;

        auto AddToken = [&](int length, SqlTokenType type)
        {
            auto tokenBegin = begin ? begin : reading;
            String value(tokenBegin, tokenBegin + length);

            switch (type)
            {
            case SqlTokenType::Identifier:
                type =
                    value == T("select") ? SqlTokenType::Select:
                    value == T("from") ? SqlTokenType::From :
                    SqlTokenType::Identifier;
                break;            
            }

            SqlToken token;
            token.TokenType = type;
            token.Row = rowNumber;
            token.Column = tokenBegin - rowBegin + 1;
            token.Value = value;

            tokens.push_back(token);
        };

        auto ReportError = [&](int length, const String& message)
        {
            auto tokenBegin = begin ? begin : reading;
            String value(tokenBegin, tokenBegin + length);
            SqlToken token;
            token.TokenType = SqlTokenType::Unknown;
            token.Row = rowNumber;
            token.Column = tokenBegin - rowBegin + 1;
            token.Value = value;            

            TokenizerError error =
            {
                token,
                message,
            };
            
            throw error;
        };

        while (Char c = *reading)
        {
            switch (state)
            {
            case State::Begin:
                switch (c)
                {
                case T(','):
                    AddToken(1, SqlTokenType::Comma);
                    break;
                case T('*'):
                    AddToken(1, SqlTokenType::Star);
                    break;
                case T(' '):
                case T('\t'):
                case T('\r'):
                    break;
                case T('\n'):
                    rowNumber++;
                    rowBegin = reading + 1;
                    break;
                default:
                    if (T('a') <= c && c <= T('z') || T('A') <= c && c <= T('Z') || c == T('_'))
                    {
                        begin = reading;
                        state = State::Identifier;
                    }
                    else
                    {
                        ReportError(1, T("Unknown character: \"") + String(reading, reading + 1) + T("\"."));
                    }
                }

                break;
            case State::Identifier:
                if (T('a') <= c && c <= T('z') || T('A') <= c && c <= T('Z') || T('0') <= c && c <= T('9') || c == T('_') || c == T('.') || c == T('-'))
                {
                    // stay in State::Identifier
                }
                else
                {
                    AddToken(reading - begin, SqlTokenType::Identifier);
                    state = State::Begin;
                    reading--;
                    begin = nullptr;
                }
                break;                
            default:
                break;
            }

            reading++;
        }

        switch (state)
        {        
        case State::Identifier:
            AddToken(reading - begin, SqlTokenType::Identifier);
            break;
        }

        return tokens;
    }
}