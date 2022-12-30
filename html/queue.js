class Queue {
    constructor() {
      this.elements = {};
      this.start = 0;
      this.end = 0;
    }
    enqueue(element) {
      this.elements[this.end] = element;
      this.end++;
    }
    dequeue() {
      const item = this.elements[this.start];
      delete this.elements[this.start];
      this.start++;
      return item;
    }
    get length() {
      return this.end - this.start;
    }
    get isEmpty() {
      return this.length === 0;
    }
  }


if (typeof module != "undefined")
{
    module.exports.default = module.exports = {Queue}
}
  
