package main

import "github.com/veandco/go-sdl2/sdl"

type Movement func(sdl.Point) sdl.Point

func isInverseMove(a, b Movement) bool {
	aux := sdl.Point{}
	return samePoint(aux, b(a(aux)))
}

func moveNone(old sdl.Point) sdl.Point {
	return old
}

func moveUp(old sdl.Point) sdl.Point {
	return sdl.Point{X: old.X, Y: old.Y - 1}
}

func moveLeft(old sdl.Point) sdl.Point {
	return sdl.Point{X: old.X - 1, Y: old.Y}
}

func moveDown(old sdl.Point) sdl.Point {
	return sdl.Point{X: old.X, Y: old.Y + 1}
}

func moveRight(old sdl.Point) sdl.Point {
	return sdl.Point{X: old.X + 1, Y: old.Y}
}
