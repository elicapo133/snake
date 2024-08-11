package main

import (
	"fmt"
	"github.com/veandco/go-sdl2/sdl"
	"math/rand"
	"os"
)

func die(str string) {
	fmt.Fprintln(os.Stderr, str)
	os.Exit(1)
}

func outOfBounds(head sdl.Point, bounds sdl.Rect) bool {
	return head.X < bounds.X ||
		head.X >= bounds.X+bounds.W ||
		head.Y < bounds.Y ||
		head.Y >= bounds.Y+bounds.H
}

func colliding(snake []sdl.Point) bool {
	for i, a := range snake {
		for j, b := range snake {
			if samePoint(a, b) && i != j {
				return true
			}
		}
	}

	return false

}

func setColor(ren *sdl.Renderer, col sdl.Color) {
	ren.SetDrawColor(col.R, col.G, col.B, col.A)
}

func drawBlock(ren *sdl.Renderer, point sdl.Point, blockSize int32) {
	rect := sdl.Rect{X: point.X * blockSize, Y: point.Y * blockSize, W: blockSize, H: blockSize}
	ren.FillRect(&rect)
}

func samePoint(a, b sdl.Point) bool {
	if a.X == b.X && a.Y == b.Y {
		return true
	}
	return false
}

func randomizeApple(rng *rand.Rand, rect sdl.Rect, snake []sdl.Point) (apple sdl.Point) {
	valid := false
	for !valid {
		valid = true
		apple = sdl.Point{X: rect.X + rng.Int31n(rect.W), Y: rect.Y + rng.Int31n(rect.H)}
		for _, s := range snake {
			if samePoint(s, apple) {
				valid = false
				break
			}
		}
	}

	return
}
