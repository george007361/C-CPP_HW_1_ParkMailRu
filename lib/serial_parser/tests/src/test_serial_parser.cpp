#include <string.h>

#include "gtest/gtest.h"

extern "C" {
#include "parser.h"
#include "graphs.h"
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(serial_parser, serial_parser_all_args_ok) {
    Graph **test_graphs = (Graph **) malloc(sizeof(Graph) * 2);
    test_graphs[0] = create_graph();
    test_graphs[1] = create_graph();
    set_graph(test_graphs[0], "test_key1", 0);
    set_graph(test_graphs[1], "test_key2", 0);
    char *text = "la-la-la test_key1 test_key2";
    
    EXPECT_EQ(parse_text(text, strlen(text), test_graphs, 2), 0);
    EXPECT_EQ(test_graphs[0]->count, 1);
    EXPECT_EQ(test_graphs[1]->count, 1);
  }