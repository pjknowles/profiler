#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <molpro/Profiler/Tree/Counter.h>
#include <molpro/Profiler/Tree/Node.h>
#include <molpro/Profiler/Tree/Profiler.h>

using molpro::profiler::tree::Profiler;

TEST(Profiler, constructor) {
  const auto description = "test";
  Profiler p(description);
  ASSERT_EQ(p.description, description);
  ASSERT_FALSE(p.counter().get_wall().dummy());
  ASSERT_TRUE(p.counter().get_cpu().dummy());
  ASSERT_TRUE(p.root);
  ASSERT_EQ(p.root->parent, nullptr);
  ASSERT_EQ(p.root->name, p.root_name);
  ASSERT_EQ(p.root, p.active_node);
  ASSERT_EQ(p.get_max_depth(), std::numeric_limits<int>::max());
  ASSERT_EQ(p.get_current_depth(), 0);
}

namespace {
void test_start(Profiler& p, const std::string& name) {
  ASSERT_EQ(p.get_current_depth(), 1);
  ASSERT_NE(p.root, p.active_node);
  ASSERT_EQ(p.active_node->parent, p.root);
  ASSERT_EQ(p.active_node->name, name);
}

void test_stop(Profiler& p) {
  ASSERT_EQ(p.get_current_depth(), 0);
  ASSERT_EQ(p.root, p.active_node);
  ASSERT_EQ(p.active_node->parent, nullptr);
}
} // namespace

TEST(Profiler, start_stop) {
  Profiler p("test");
  const auto name = "level 1";
  p.start(name);
  test_start(p, name);
  p.stop();
  test_stop(p);
}

TEST(Profiler, push) {
  Profiler prof("test");
  {
    const auto name = "level 1";
    auto proxy = prof.push(name);
    test_start(prof, name);
  }
  test_stop(prof);
}

namespace {
void construct_stack(Profiler& p, int n) {
  for (size_t i = 0; i < n; ++i)
    p.start("level " + std::to_string(i));
}

int measure_depth(const Profiler& p) {
  int n = 0;
  auto node = p.active_node;
  while (node->parent) {
    node = node->parent;
    ++n;
  }
  return n;
}
} // namespace

TEST(Profiler, start__stop_all__stack) {
  constexpr int depth = 4;
  Profiler p("");
  construct_stack(p, depth);
  ASSERT_EQ(p.get_current_depth(), depth);
  ASSERT_EQ(measure_depth(p), depth);
  p.stop_all();
  test_stop(p);
  ASSERT_EQ(measure_depth(p), 0);
}

TEST(Profiler, max_depth) {
  constexpr int max_depth = 2, depth = max_depth + 2;
  Profiler p("");
  p.set_max_depth(max_depth);
  ASSERT_EQ(p.get_max_depth(), max_depth);
  construct_stack(p, depth);
  ASSERT_EQ(p.get_current_depth(), depth);
  ASSERT_EQ(measure_depth(p), max_depth);
}

TEST(Profiler, counter) {
  Profiler p("");
  ASSERT_NO_FATAL_FAILURE(p.counter());
  auto& c = p.counter();
  ASSERT_FALSE(c.get_wall().stopped());
}
