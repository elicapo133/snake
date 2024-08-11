package main

import (
	"github.com/veandco/go-sdl2/sdl"
	"math/rand"
)

const (
	MaxMovementQueueSize = 3
)

// States
const (
	Running = iota
	Paused
	Finished
)

// Window globals
var (
	window   *sdl.Window
	renderer *sdl.Renderer

	windowWidth  int32
	windowHeight int32
	gameBounds   sdl.Rect

	rng *rand.Rand
)

// Game globals
var (
	config        Config
	snake         []sdl.Point
	apple         sdl.Point
	state         int = Paused
	movementQueue []Movement
	direction     = moveDown
	applesEaten   = 0
)

// Colors
var (
	bgColor        = sdl.Color{}
	borderColor    = sdl.Color{B: 0x70, A: 0xff}
	snakeColor     = sdl.Color{G: 0x70, A: 0xff}
	snakeHeadColor = sdl.Color{G: 0xaa, A: 0xff}
	appleColor     = sdl.Color{R: 0xaa, A: 0xff}
)
