--- basic program

function init()
   size = 15
   t = 0
end

function update()
   t = t + 0.01
   size = 15 * math.sin(t);
end

function draw()
   clear(55, 55, 225)
   color(200, 150, 25)
   text('Hello, MoonCon User!', 1)
   rect(40, 40, size)
end
