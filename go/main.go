package main

import (
	"encoding/json"
	"fmt"
	"github.com/veandco/go-sdl2/sdl"
	"io/ioutil"
	"log"
	"math/rand"
	"os"
	"time"
)

func start() {
	// Initialize SDL
	err := sdl.Init(sdl.INIT_EVERYTHING)
	if err != nil {
		die(err.Error())
	}

	// Initialize config
	jsonFile, err := os.Open("config.json")
	if err != nil {
		log.Fatalf("Error opening config file: %v", err)
	}
	defer jsonFile.Close()

	byteValue, _ := ioutil.ReadAll(jsonFile)

	// Parse the JSON file into the Config struct
	err = json.Unmarshal(byteValue, &config)
	if err != nil {
		log.Fatalf("Error parsing config file: %v", err)
	}

	windowWidth, windowHeight = (config.GameWidth+2)*config.BlockSize, (config.GameHeight+2)*config.BlockSize

	// Create window
	gameBounds = sdl.Rect{X: 1, Y: 1, W: config.GameWidth, H: config.GameHeight}
	window, err = sdl.CreateWindow("DEMO",
		sdl.WINDOWPOS_CENTERED, sdl.WINDOWPOS_CENTERED,
		windowWidth, windowHeight, 0)
	if err != nil {
		die(err.Error())
	}

	// Create renderer
	renderer, err = sdl.CreateRenderer(window, -1, 0)
	if err != nil {
		os.Exit(1)
	}

	// Init rand
	rng = rand.New(rand.NewSource(time.Now().UTC().UnixNano()))

	// Init snake
	snake = []sdl.Point{
		{gameBounds.W/2 + 1, gameBounds.H/2 + 1},
	}

	// Init apple
	apple = randomizeApple(rng, gameBounds, snake)
}

func input() {
	for event := sdl.PollEvent(); event != nil; event = sdl.PollEvent() {
		switch event.(type) {
		case *sdl.QuitEvent:
			state = Finished
			break
		case *sdl.KeyboardEvent:
			if event.(*sdl.KeyboardEvent).Type != sdl.KEYDOWN {
				continue
			}

			possibleDirection := direction

			switch event.(*sdl.KeyboardEvent).Keysym.Sym {
			case sdl.K_w, sdl.K_k, sdl.K_UP:
				possibleDirection = moveUp
			case sdl.K_a, sdl.K_h, sdl.K_LEFT:
				possibleDirection = moveLeft
			case sdl.K_s, sdl.K_j, sdl.K_DOWN:
				possibleDirection = moveDown
			case sdl.K_d, sdl.K_l, sdl.K_RIGHT:
				possibleDirection = moveRight
			default:
				continue
			}

			state = Running

			if len(movementQueue) < MaxMovementQueueSize {
				if len(movementQueue) > 0 {
					if isInverseMove(movementQueue[len(movementQueue)-1], possibleDirection) {
						return
					}
				} else if isInverseMove(possibleDirection, direction) {
					return
				}

				movementQueue = append(movementQueue, possibleDirection)

			}

		}
	}
}

func draw() {
	// Fill bg
	setColor(renderer, bgColor)
	if err := renderer.Clear(); err != nil {
		die(err.Error())
	}

	// Render borders
	marginSize := config.BlockSize / 2
	setColor(renderer, borderColor)
	if err := renderer.FillRect(&sdl.Rect{W: windowWidth, H: marginSize}); err != nil {
		die(err.Error())
	}
	if err := renderer.FillRect(
		&sdl.Rect{Y: marginSize, W: marginSize, H: windowHeight - marginSize}); err != nil {
		die(err.Error())
	}
	if err := renderer.FillRect(
		&sdl.Rect{X: marginSize, Y: windowHeight - marginSize, W: windowWidth - marginSize, H: config.BlockSize}); err != nil {
		die(err.Error())
	}
	if err := renderer.FillRect(
		&sdl.Rect{X: windowWidth - marginSize, Y: marginSize, W: marginSize, H: windowHeight - 2*marginSize}); err != nil {
		die(err.Error())
	}

	// Render apple
	setColor(renderer, appleColor)
	drawBlock(renderer, apple, config.BlockSize)

	// Render snake
	setColor(renderer, snakeColor)
	for i := 1; i < len(snake); i++ {
		drawBlock(renderer, snake[i], config.BlockSize)
	}
	setColor(renderer, snakeHeadColor)
	drawBlock(renderer, snake[0], config.BlockSize)

	renderer.Present()
}

func update() {
	if state == Paused {
		return
	}

	if direction != nil && len(movementQueue) > 0 {
		direction = movementQueue[0]
		movementQueue = movementQueue[1:]
	}

	if samePoint(apple, direction(snake[0])) {
		snake = append([]sdl.Point{apple}, snake...)
		apple = randomizeApple(rng, gameBounds, snake)
		applesEaten++
		return
	}

	for i := len(snake) - 1; i >= 1; i-- {
		snake[i] = snake[i-1]
	}
	snake[0] = direction(snake[0])

	if outOfBounds(snake[0], gameBounds) || colliding(snake) {
		state = Finished
	}

}

func quit() {
	renderer.Destroy()
	window.Destroy()
	sdl.Quit()
	if applesEaten == 0 {
		fmt.Println("You ate no apples :(")
	} else if applesEaten == 1 {
		fmt.Println("Well... you ate 1 apple.")
	} else {
		fmt.Println("You ate", applesEaten, "apples!")
	}
	os.Exit(0)
}

func main() {
	start()

	go func() {
		for state != Finished {
			input()
		}
		quit()
	}()

	for {
		update()
		draw()
		sdl.Delay(config.Delay)
	}

}
