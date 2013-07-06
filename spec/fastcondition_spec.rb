require 'spec_helper'

describe FastCondition do
  context "#signal" do
    it "returns self if nothing to signal" do
      subject.signal.should == subject
    end

    it "returns self if something is waiting for a signal" do
      mutex = Mutex.new
      thread = Thread.new do
        m.synchronize do
          subject.wait(mutex)
        end
      end

      # ensures that th grabs m before current thread
      Thread.pass while thread.status and thread.status != "sleep"

      mutex.synchronize { subject.signal }.should == subject

      thread.join
    end
  end
end
