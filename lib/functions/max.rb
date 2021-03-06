class Maximum < Base
  Arbol.add_mapped_class(
    'max', 
    Maximum,
%{void maximum(long op1[3], long op2[3], long out[3]) {
  out[0] = max(op1[0], op2[0]);
  out[1] = max(op1[1], op2[1]);
  out[2] = max(op1[2], op2[2]);
}}
  )
  
  attr_accessor :op1
  attr_accessor :op2
  
  def param_keys
    [:op1, :op2]
  end

  def arduino_code
    unless @frame_optimized 
      [
        "maximum(#{@op1.name}, #{@op2.name}, #{@name});"
      ]
    else
      []
    end
  end
  
  def cycle_level_arduino_code
    if @frame_optimized
      [
        "maximum(#{@op1.name}, #{@op2.name}, #{@name});"
      ]
    else
      []
    end
  end

  def top_level_scope_code
    [
      "long #{@name}[3];"
    ]
  end
end

module Arbol
  class Documentation

  def max
%{--
### max(left, right)

* **operator1**
* **operator2**

Maximum (greater) of the two operators.

}
  end

  end
end

def max(op1, op2)
  h = ArbolHash.new
  h[:type] = 'max'
  h[:op1] = resolve(op1)
  h[:op2] = resolve(op2)
  h
end