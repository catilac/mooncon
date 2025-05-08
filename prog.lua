--- basic program

function init()
   size = 15
end

function update()
   print('update')
end

function draw()
   clear(55, 55, 225)
   color(200, 150, 25)
   text('Hello, MoonCon User!', 1)
   rect(40, 40, size)
end
