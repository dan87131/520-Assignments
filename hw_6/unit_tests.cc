#include "gtest/gtest.h"
#include "Stopwatch.h"
#include "RandomProcess.h"
#include "Filter.h"
#include "thread"
#include "Integrator.h"

class ProcessValues : public Process {
public:
  ProcessValues(std::string name) : Process(name) {}
  void init() {}
  void start() { _count = 0; }
  void update() {
    if (_count % 2 == 0) {
      channel("link").send(0.25);
    } else {
      channel("link").send(0.75);
    }
    _count++;
  }
  void stop() {}
private:
  int _count;
};


namespace {

  TEST(StopwatchTest, ElapsedTime) {
  Stopwatch stopwatch;
  stopwatch.start();

  // Wait for some time
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  stopwatch.stop();
  double elapsed_time_ms = stopwatch.get_milliseconds();
  double elapsed_time_min = stopwatch.get_minutes();
  double elapsed_time_s = stopwatch.get_seconds();
  double elapsed_time_ns = stopwatch.get_nanoseconds();
  EXPECT_NEAR(elapsed_time_ms, 500, 50);
  EXPECT_NEAR(elapsed_time_s, 0.5, 0.1);
  EXPECT_NEAR(elapsed_time_ns, 500000000, 100000000);
  EXPECT_EQ(elapsed_time_min, 0);
}

TEST(StopwatchTest, Reset) {
  Stopwatch stopwatch;
  stopwatch.start();

  // Wait for some time
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  stopwatch.reset();

  double elapsed_time_ms = stopwatch.get_milliseconds();
  EXPECT_EQ(elapsed_time_ms, 0);
}

TEST(StopwatchTest, MultipleStartStop) {
  Stopwatch stopwatch;
  stopwatch.start();

  // Wait for some time
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  stopwatch.stop();
  double elapsed_time_ms1 = stopwatch.get_milliseconds();

  stopwatch.start();

  // Wait for some more time
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  stopwatch.stop();
  double elapsed_time_ms2 = stopwatch.get_milliseconds();

  double total_elapsed_time_ms = elapsed_time_ms1 + elapsed_time_ms2;

  EXPECT_NEAR(total_elapsed_time_ms, 1000, 50);
}


TEST(FilterTest, ProcessValues) {
  Manager m;
  ProcessValues pv("process values");
  Filter f("filter");
  Channel link("link");

  m.schedule(pv, 1_ms)
   .schedule(f, 1_ms)
   .add_channel(link)
   .init()
   .run(100_ms);

  double val = f.value();
  EXPECT_EQ(val, 0.5);
}

TEST(RandomProcessTest, RandomNumbers) {
  Manager m;
  RandomProcess r("random numbers");
  Filter f("filter");
  Channel link("link");

  m.schedule(r, 1_ms)
   .add_channel(link)
   .init()
   .run(10_ms);

  double val = link.latest();
  EXPECT_GE(val, 0);
  EXPECT_LE(val, 1);
}

// Test the Integrator process with a constant input
TEST(IntegratorTest, ConstantInput)
{
    // Instantiate the Integrator process
    Integrator integrator;
    double delta_time = 1.0;
    integrator.set_delta_time(delta_time);

    // Set up the test input
    double input = 2.0;
    int num_samples = 5;

    // Compute the expected output
    double expected = input * num_samples * delta_time;

    // Apply the input to the Integrator process
    for (int i = 0; i < num_samples; i++) {
        integrator.update(input);
    }

    // Check the result
    double actual = integrator.value();
    double tolerance = 1e-6;
    ASSERT_NEAR(expected, actual, tolerance);
}

// Test the Integrator process with a linear input
TEST(IntegratorTest, LinearInput)
{
    // Instantiate the Integrator process
    Integrator integrator;
    double delta_time = 1.0;
    integrator.set_delta_time(delta_time);

    // Set up the test input
    double input = 2.0;
    int num_samples = 5;

    // Compute the expected output
    double expected = (0.5 * input) * num_samples * (num_samples + 1) * delta_time;

    // Apply the input to the Integrator process
    for (int i = 0; i < num_samples; i++) {
        integrator.update(input * (i + 1));
    }

    // Check the result
    double actual = integrator.value();
    double tolerance = 1e-6;
    ASSERT_NEAR(expected, actual, tolerance);
}


}  // namespace
