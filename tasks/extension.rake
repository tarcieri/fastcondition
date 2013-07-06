unless defined? JRUBY_VERSION
  require 'rake/extensiontask'
  Rake::ExtensionTask.new('fastcondition_ext') do |ext|
    ext.ext_dir = 'ext/fastcondition'
  end
end
