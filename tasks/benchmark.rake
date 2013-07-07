task :benchmark do
  require "benchmark/ips"
  require "fastcondition"

  def signal_benchmark(klass, iterations)
    mutex = Mutex.new
    cond  = klass.new
    n     = 0

    thread = Thread.new do
      running = true
      while running
        mutex.lock
        cond.wait(mutex)
        n += 1
        running = false if n >= iterations
        mutex.unlock
      end
    end

    loop do
      value = nil
      mutex.lock
      value = n
      cond.signal if value < iterations
      mutex.unlock
      break if value >= iterations
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