# reason-cognito

This library is a work in progress!

## Development outline

MVP Development Path:

A. Bucklescript Only

1. [ ] Client Operations
   - [x] SignUp
   - [x] Confirm SignUp
   - [x] InitiateAuth
     - [x] Password Flow
     - [ ] SRP Flow
   - [x] Change Password
   - [x] Reset Password
2. [x] Modularization
   - [x] Identify Sharable Core
   - [ ] Extract to Separte Modules
   - [ ] Remove all Bucklescript Dependencies from Core
   - [ ] Consume core (tightly bound by bs module)

B. High Level Library

1. [ ] Storage Layer
2. [ ] ReasonReact hooks

C. Ocaml/Native

1. [ ] Consume Core for Native (tighly bound)
2. ???
3. [ ] Consume Core for Native (injected Dependencies)

# Build

```
npm run build
```

# Watch

```
npm run watch
```

# Editor

If you use `vscode`, Press `Windows + Shift + B` it will build automatically# reason-cognito
