# coding: utf-8

Gem::Specification.new do |spec|
  spec.name          = "fastcondition"
  spec.version       = "1.0.0.pre"
  spec.authors       = ["Tony Arcieri"]
  spec.email         = ["tony.arcieri@gmail.com"]
  spec.description   = "Faster ConditionVariables for MRI"
  spec.summary       = "A reimplementation of ConditionVariable that uses pthread conditions"
  spec.homepage      = "https://github.com/tarcieri/fastcondition"
  spec.license       = "MIT"

  spec.files         = `git ls-files`.split($/)
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib"]

  spec.add_development_dependency "bundler", "~> 1.3"
  spec.add_development_dependency "rake"
  spec.add_development_dependency "rake-compiler"
  spec.add_development_dependency "rspec"
end
