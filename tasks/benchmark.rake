task :benchmark do
  require "benchmark/ips"
  require "fastcondition"

  def signal_benchmark(klass, iterations)
    mutex = Mutex.new
    cond  = klass.new

    mutex.synchronize do
      Thread.new do
        iterations.times { |n| mutex.synchronize { cond.signal } }
      end

      iterations.times { cond.wait(mutex) }
    end
  end

  Benchmark.ips do |ips|
    ips.report("ConditionVariable") do |n|
      signal_benchmark(ConditionVariable, n)
    end

    ips.report("FastCondition") do |n|
      signal_benchmark(FastCondition, n)
    end
  end
end