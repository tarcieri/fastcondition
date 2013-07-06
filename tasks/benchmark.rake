task :benchmark do
  require "benchmark/ips"
  require "fastcondition"

  def signal_benchmark(klass, iterations)
    mutex = Mutex.new
    cond  = klass.new

    mutex.lock

    Thread.new do
      iterations.times do
        mutex.lock
        begin
          cond.signal
        ensure
          mutex.unlock
        end
      end
    end

    iterations.times do
      cond.wait(mutex)
    end

    mutex.unlock
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