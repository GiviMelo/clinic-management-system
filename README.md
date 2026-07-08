# Clinic Management System

A command-line patient management system for dental clinics, developed in C.
Built as a personal project during the first semester of Computer Science at PUC Minas.

## About

This project was created to help manage patients at a dental clinic, allowing the user to store and manipulate patient data persistently through files. The system runs entirely in the terminal and was built using only the C standard library.

## Features

- Register new patients
- List all registered patients
- View a specific patient's details
- Edit patient information
- Delete patients
- Persistent storage — data is saved between sessions

## Patient Data

Each patient stores the following information:

- ID (auto-generated)
- Name
- Age
- Phone number
- Email

## How to Compile and Run

Make sure you have `gcc` installed, then run:

```bash
gcc main.c -o main
./main
```

## Project Structure

```
clinic-management-system/
├── main.c          # Main source code
├── data/           # Patient data files (generated at runtime)
└── .gitignore
```

## Roadmap

- [ ] Record consultations per patient
- [ ] Search patients by name
- [ ] Organize data into a dedicated folder

## Author

**Lucas Givisiez**
Computer Science student at PUC Minas
