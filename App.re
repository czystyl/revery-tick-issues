open Revery;
open Revery.UI;
open Revery.Math;
open Revery.UI.Components;

module Brick = {
  let make = (~active: bool, ()) => {
    <Image
      opacity={active ? 1. : 0.1}
      src="brick.png"
      width=22
      height=22
      style=Style.[margin(1)]
    />;
  };
};

module Car = {
  type field =
    | Filled
    | Empty;

  let car = [
    [Empty, Filled, Empty],
    [Filled, Filled, Filled],
    [Empty, Filled, Empty],
    [Filled, Empty, Filled],
  ];

  let make = () => {
    <View>
      {car
       |> List.map(row => {
            <View style=Style.[flexDirection(`Row)]>
              {row
               |> List.map(f =>
                    <Brick
                      active={
                        switch (f) {
                        | Filled => true
                        | Empty => false
                        }
                      }
                    />
                  )
               |> React.listToElement}
            </View>
          })
       |> React.listToElement}
    </View>;
  };
};

module Game = {
  open Car;

  let boardHeight = 20;
  let createList = index => {
    let baseLine = Array.make(8, Car.Empty) |> Array.to_list;

    index mod 3 === 0
      ? [Car.Empty] @ baseLine @ [Car.Empty]
      : [Car.Filled] @ baseLine @ [Car.Filled];
  };

  let o = Empty;
  let x = Filled;

  let left = [
    [x, o, o, x, o, o, o, o, o, x],
    [x, o, x, x, x, o, o, o, o, x],
    [o, o, o, x, o, o, o, o, o, o],
    [x, o, x, o, x, o, o, o, o, x],
  ];

  let right = [
    [x, o, o, o, o, o, x, o, o, x],
    [x, o, o, o, o, x, x, x, o, x],
    [o, o, o, o, o, o, x, o, o, o],
    [x, o, o, o, o, x, o, x, o, x],
  ];

  let empty = [
    [x, o, o, o, o, o, o, o, o, x],
    [x, o, o, o, o, o, o, o, o, x],
    [o, o, o, o, o, o, o, o, o, o],
    [x, o, o, o, o, o, o, o, o, x],
  ];

  let nextState = board => {
    let nextBoard = List.tl(board);

    if (List.length(nextBoard) <= boardHeight) {
      nextBoard @ empty @ left @ empty @ empty @ right @ empty;
    } else {
      nextBoard;
    };
  };

  let createBoard = () => {
    Array.make(boardHeight, None)
    |> Array.mapi((i, _) => createList(i))
    |> Array.to_list;
  };
};

module Counter = {
  let%component make = () => {
    let%hook (count, setCount) = Hooks.state(0);

    let _ =
      Tick.interval(_ => {setCount(c => c + 1)}, Time.milliseconds(50));
    <View>
      <Text
        text={string_of_int(count)}
        style=Style.[fontFamily("Roboto-Regular.ttf"), fontSize(20)]
      />
    </View>;
  };
};

module Board = {
  let%component make = () => {
    let%hook (game, setGame) = React.Hooks.state(Game.createBoard());
    let%hook (count, setCount) = Hooks.state(0);

    let _ =
      Tick.interval(
        _ => {
          setCount(c
            => c + 1)
            /*      setGame(Game.nextState);*/
        },
        Time.milliseconds(50),
      );

    /*
         let%hook () =
           Hooks.effect(
             Always,
             () => {
               let dispose =
                 Revery_Core.Tick.timeout(
                   _ => {
                     setCount(c => c + 1);
                     setGame(Game.nextState);
                   },
                   Time.milliseconds(100),
                 );

               Some(dispose);
             },
           );
     */
    <View>
            <Text
              text={string_of_int(count)}
              style=Style.[fontFamily("Roboto-Regular.ttf"), fontSize(20)]
            />
          </View> /*     {game
       |> List.rev
       |> List.mapi((x, row) =>
            if (List.length(game) - x > Game.boardHeight) {
              React.empty;
            } else {
              <View style=Style.[flexDirection(`Row)]>
                {row
                 |> List.map(f => {
                      <Brick
                        active={
                          switch (f) {
                          | Car.Filled => true
                          | Car.Empty => false
                          }
                        }
                      />
                    })
                 |> React.listToElement}
              </View>;
            }
         )
       |> React.listToElement}
   */;
  };
};

let init = app => {
  let _ = Revery.Log.listen((_, msg) => print_endline("LOG: " ++ msg));

  let win = App.createWindow(app, "Welcome to Revery!");

  let containerStyle =
    Style.[
      position(`Absolute),
      justifyContent(`Center),
      alignItems(`Center),
      bottom(0),
      top(0),
      left(0),
      right(0),
      backgroundColor(Colors.gray),
    ];

  let innerStyle = Style.[flexDirection(`Row), alignItems(`FlexEnd)];

  let element =
    <View style=containerStyle> <View style=innerStyle /> <Counter /> </View>;

  let _ = UI.start(win, element);

  ();
};

App.start(init);
