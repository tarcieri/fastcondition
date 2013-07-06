# FastCondition

The `FastCondition` class is a reimplementation of the ConditionVariable API
using pthread conditions (where available).

FastCondition only works on MRI when compiled on an OS that implements the
pthreads API. If installed on a Ruby that isn't MRI, or on an OS that doesn't
support pthreads, the gem will simply use `ConditionVariable` as the
`FastCondition` implementation.

This means that FastCondition will work on all Rubies, even JRuby, but will use
native ConditionVariable implementations on these platforms.

## Installation

Add this line to your application's Gemfile:

    gem 'fastcondition'

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install fastcondition

## Usage

TODO: Write usage instructions here

## Contributing

1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request
