#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

typedef enum
{
    TO_UPPER, TO_LOWER, REPLACE, UNKNOWN
} CommandType;

struct Command
{
    CommandType type;
    int number;
    string *args; 

    Command(CommandType _type, int _number, int args_size=0) : type(_type), number(_number)
    {
        args = new string[args_size];
    }

    Command(Command &other) : type(other.type), number(other.number), args(other.args)
    {
        other.args = nullptr;
    }

    ~Command()
    {
        delete[] args;
    }
};

Command parse_command(const char *command)
{
    string comm = string(command);
    int sep_idx = comm.find_first_of(':');

    int line_num = std::stoi(comm.substr(0, sep_idx + 1));

    string args = comm.substr(sep_idx + 1);
    CommandType type;
    int args_num = 0;

    switch (args[0])
    {
        case 'u':
            type = TO_LOWER;
            break;
        case 'U':
            type = TO_UPPER;
            break;
        case 'R':
            type = REPLACE;
            args_num = 1;
            break;
        default:
            type = UNKNOWN;
    }

    Command result(type, line_num, args_num);
    if (result.type == REPLACE)
    {
        result.args[0] = args.substr(1);
    }

    return result;
}

int main(int argc, char *argv[])
{
    // TODO: check if the first argument is valid

    // Opening file, reading it into memory, closing file
    ifstream input_file(argv[1]);
    vector<string> fields;

    if (input_file.is_open())
    {
        string line;
        while (std::getline(input_file, line, '\t'))
        {
            fields.push_back(line);
        }
        input_file.close();
    } else
    {
        cerr << "Error reading file" << endl;
    }

    // Changing fields stored in a vector in memory
    for (int idx = 2; idx < argc; ++idx)
    {
        Command command = parse_command(argv[idx]);
        int line_num = command.number;
        // TODO: check command.number

        switch (command.type)
        {
            case TO_UPPER:
                for (long unsigned i = 0; i < fields[line_num].length(); ++i)
                {
                    fields[line_num][i] = toupper(fields[line_num][i]);
                }
                break;
            case TO_LOWER:
                for (long unsigned i = 0; i < fields[line_num].length(); ++i)
                {
                    fields[line_num][i] = tolower(fields[line_num][i]);
                }
                break;
            case REPLACE:
                cout << command.args[0] << endl;
                for (long unsigned i = 0; i < fields[line_num].length(); ++i)
                {
                    if (fields[line_num][i] == command.args[0][0])
                    {
                        fields[line_num][i] = command.args[0][1];
                    }
                }
                break;
            default:
                cerr << "Command type is not recognized" << endl;
        }

        if (command.type != UNKNOWN) {
            cout << fields[command.number] << endl;
        }
    }

    // Opening file, flushing values into it, closing file
    ofstream output_file(argv[1]);
    if (output_file.is_open())
    {
        for (const auto &val : fields)
        {
            output_file << val << '\t';
        }
        output_file.close();
    } else
    {
        cerr << "Error writing file" << endl;
    }
    
    return 0;
}
