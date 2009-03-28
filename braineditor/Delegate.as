/*

Delegate.as v1.0.2

Copyright (c) 2005 Steve Webster

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

class Delegate {
    public static function create(target:Object, handler:Function):Function {
        // Get any extra arguments for handler
        var extraArgs:Array = arguments.slice(2);
        
        // Create delegate function
        var delegate:Function = function() {
            // Get reference to self
            var self:Function = arguments.callee;
            
            // Augment arguments passed from broadcaster with additional args
            var fullArgs:Array = arguments.concat(self.extraArgs, [self]);
            
            // Call handler with arguments
            return self.handler.apply(self.target, fullArgs);
        };
        
        // Pass in local references
        delegate.extraArgs = extraArgs;
        delegate.handler = handler;
        delegate.target = target;
        
        // Return the delegate function.
        return delegate;
    }
}
