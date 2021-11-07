// / <copyright>© George Illarionov 2021. All rights ignored</copyright>
#include "gtest/gtest.h"

extern "C" {
#include "graphs.h"
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(create_graph, create_graph) {
  Graph *new_graph = create_graph();
  EXPECT_TRUE(new_graph);
  EXPECT_FALSE(new_graph->key);
  EXPECT_EQ(new_graph->count, 0);
}

TEST(set_graph_key, set_graph_key_right) {
  Graph *new_graph = create_graph();
  EXPECT_TRUE(set_graph_key(new_graph, "test_key"));
  EXPECT_EQ(new_graph->count, 0);
  EXPECT_STREQ(new_graph->key, "test_key");
}

TEST(set_graph_key, set_graph_key_fail) {
  Graph *new_graph = create_graph();
  EXPECT_FALSE(set_graph_key(new_graph, NULL));
  EXPECT_EQ(new_graph->count, 0);
  EXPECT_FALSE(new_graph->key);
}

TEST(set_graph_count, set_graph_count) {
  Graph *new_graph = create_graph();
  EXPECT_TRUE(set_graph_count(new_graph, 666));
  EXPECT_EQ(new_graph->count, 666);
  EXPECT_FALSE(new_graph->key);
}

TEST(set_graph, set_graph_right) {
  Graph *new_graph = create_graph();
  EXPECT_TRUE(set_graph(new_graph, "test_graph", 666));
  EXPECT_EQ(new_graph->count, 666);
  EXPECT_STREQ(new_graph->key, "test_graph");
}

TEST(set_graph, set_graph_fail) {
  Graph *new_graph = create_graph();
  EXPECT_FALSE(set_graph(new_graph, NULL, 666));
}

TEST(clear_graph, clear_graph) {
  Graph *new_graph = create_graph();
  EXPECT_TRUE(set_graph(new_graph, "test_graph", 666));
  EXPECT_TRUE(clear_graph(new_graph));
  EXPECT_EQ(new_graph->count, 0);
  EXPECT_FALSE(new_graph->key);
}

TEST(free_graph, free_graph) {
  Graph *new_graph = create_graph();
  EXPECT_TRUE(set_graph(new_graph, "test_graph", 666));
  free_graph(new_graph);
  EXPECT_TRUE(new_graph);
}

TEST(free_graphs, free_graphs) {
  Graph *g1 = create_graph(), *g2 = create_graph();
  set_graph(g1, "1", 1);
  set_graph(g2, "2", 2);
  size_t count = 2;
  Graph **graphs = (Graph **)malloc(sizeof(Graph *) * count);
  graphs[0] = g1;
  graphs[1] = g2;
  free_graphs(&graphs, &count);
  EXPECT_FALSE(graphs);
  EXPECT_EQ(count, 0);
}

TEST(interprier, interprier) {
  long delta = 0;
  Graph **graphs;
  const char key[] = "test key";
  EXPECT_TRUE(interprier(NULL, graphs, 1, key, key));
  EXPECT_TRUE(interprier(&delta, NULL, 1, key, key));
  EXPECT_TRUE(interprier(&delta, graphs, 1, NULL, key));
  EXPECT_TRUE(interprier(&delta, graphs, 1, key, NULL));
}
