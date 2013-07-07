task :benchmark do
  require "benchmark/ips"
  require "fastcondition"

  class Trigger
    def self.benchmark(condition_class, n)
      trigger = new(condition_class)
      thread = Thread.new { n.times { trigger.wait } }
      n.times { trigger.fire }
      thread.join
    end

    def initialize(condition_class)
      @mutex = Mutex.new
      @cond  = condition_class.new

      @fired_count    = 0
      @consumed_count = 0
    end

    def fire
      @mutex.synchronize do
        @cond.signal if @fired_count <= @consumed_count
        @fired_count += 1
      end
    end

    def wait
      @mutex.synchronize do
        @cond.wait(@mutex) if @fired_count <= @consumed_count
        @consumed_count += 1
      end
    end
  end

  Benchmark.ips do |ips|
    ips.report("ConditionVariable") do |n|
      Trigger.benchmark(ConditionVariable, n)
    end

    ips.report("FastCondition") do |n|
      Trigger.benchmark(FastCondition, n)
    end
  end
end