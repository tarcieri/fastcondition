require 'thread'

if RUBY_ENGINE == "ruby"
  require "fastcondition_ext"
end

if !defined?(FastCondition)
  FastCondition = ConditionVariable
end
