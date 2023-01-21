#include "BulkPrinters.h"
#include "InputCommandParser.h"
#include "TestCommandParser.h"

void test(std::shared_ptr<CommandParser> parser)
{
    Bulk bulk(parser->get_bulk_size());

    std::shared_ptr<ConsoleBulkPrinter> cbp = ConsoleBulkPrinter::create(&bulk);
    std::shared_ptr<FileBulkPrinter> fbp = FileBulkPrinter::create(&bulk);

    parser->parse(bulk);
}

int main(int argc, const char* argv[])
{
    std::shared_ptr<CommandParser> cmdParser;

    if (argc > 1)
    {
        std::cout << "Test1 (static):" << std::endl;
        std::vector<std::string> test_static = { "cmd1", "cmd2", "cmd3", "cmd4", "cmd5", "EOF" };
        cmdParser = std::make_shared<TestCommandParser>(atoi(argv[1]), test_static);
        test(cmdParser);

        std::cout << std::endl << "Test2 (dynamic):" << std::endl;
        std::vector<std::string> test_dynamic = { "cmd1", "cmd2", "{", "cmd3", "cmd4", "}", "{", "cmd5", "cmd6","{", "cmd7", "cmd8", "}", "cmd9", "}", "{", "cmd10", "cmd11","EOF" };
        cmdParser = std::make_shared<TestCommandParser>(atoi(argv[1]), test_dynamic);
        test(cmdParser);
    }
    else
    {
        cmdParser = std::make_shared<InputCommandParser>();
        test(cmdParser);
    }
}

